## needle
Threads have access to everything what can be reached by the process. For example, opened files. That is how I am going to get the flag, from file `flag.txt` opened by the process. 
To see it and keep my name safety at the same time, I will return the pointer to the flag as the pointer to name. According to the `Meeting.c` code, name will be printed as it is. So I see in the consol the following: `Polythread has your concent and name: threadctf{Sh0Otin9_somE_A7omiC_F3Et}`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *meeting(void *arg)
{
    (void)arg;

    char *name = malloc(256);
    if (name == NULL)
        return NULL;

    FILE *f = fopen("/root/flag.txt", "r");
    if (f == NULL) {
        exit (1);
    }
    if (fgets(name, 256, f) == NULL) {
        strcpy(name, "empty");
    }
    fclose(f);
    return name;
}
```

## Princess
We need to kill parents of all Zombie processes which has the name started with `princess`. Look for the by command `ps -eo pid,ppid,stat,comm | grep ' Z '`
Zombie processes creates sometimes, we need to kill 50 of such processes. After it the flag will occur in home directory

## Princess2
Well, we don't gave access to the bash. So do it by code

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

static int read_stat(pid_t pid, pid_t *ppid, char *state) {
    char path[64], buf[1024];
    snprintf(path, sizeof path, "/proc/%d/stat", pid);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    if (!fgets(buf, sizeof buf, f)) { fclose(f); return -1; }
    fclose(f);
    char *p = strrchr(buf, ')');
    if (!p) return -1;
    p++;
    while (*p == ' ') p++;
    if (!*p) return -1;
    *state = *p++;
    while (*p == ' ') p++;
    *ppid = (pid_t)atoi(p);
    return 0;
}

int main(int argc, char **argv) {
    int target = (argc > 1) ? atoi(argv[1]) : 100;
    if (target <= 0) target = 100;

    prctl(PR_SET_CHILD_SUBREAPER, 1);

    pid_t me = getpid();
    int killed = 0;

    while (killed < target) {
        DIR *d = opendir("/proc");
        if (!d) return 1;

        pid_t *cand = NULL;
        int nc = 0, cap = 0;
        struct dirent *e;

        while ((e = readdir(d))) {
            if (!isdigit((unsigned char)e->d_name[0])) continue;
            pid_t p = (pid_t)atoi(e->d_name);
            if (p <= 0 || p == me) continue;

            pid_t ppid; char st;
            if (read_stat(p, &ppid, &st) != 0) continue;
            if (st != 'Z') continue;
            if (ppid <= 1 || ppid == me) continue;

            int seen = 0;
            for (int j = 0; j < nc; j++)
                if (cand[j] == ppid) { seen = 1; break; }
            if (seen) continue;

            if (nc == cap) {
                cap = cap ? cap * 2 : 16;
                pid_t *tmp = realloc(cand, cap * sizeof(pid_t));
                if (!tmp) { free(cand); closedir(d); return 1; }
                cand = tmp;
            }
            cand[nc++] = ppid;
        }
        closedir(d);

        int progress = 0;
        for (int j = 0; j < nc && killed < target; j++) {
            pid_t pp = cand[j];
            pid_t rpar; char rst;
            if (read_stat(pp, &rpar, &rst) != 0) continue;

            if (kill(pp, SIGKILL) == 0) {
                fprintf(stderr, "killed %d\n", pp);
                killed++;
                progress = 1;
            } else if (errno != ESRCH) {
                fprintf(stderr, "kill %d: %s\n", pp, strerror(errno));
            }
        }
        free(cand);
        while (waitpid(-1, NULL, WNOHANG) > 0) {}

        if (!progress) usleep(100000);
    }

    fprintf(stderr, "done: killed=%d\n", killed);
    return 0;
}
```

And after running I realized, that I also can't read the file with flag) So one more small script for reading

```c
#include <stdio.h>

int main(void) {
    FILE *f = fopen("/home/ctf/flag.txt", "r");
    int c;
    while ((c = fgetc(f)) != EOF) {
        putchar(c);
	}
    fclose(f);
    return 0;
}
```