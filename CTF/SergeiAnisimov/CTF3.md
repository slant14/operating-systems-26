# CTF 3 Sergei Anisimov CSE-04

**Threads CTF (`threadctf`) · Instance `findingnemo` · 3 of 6 challenges solved**

The commands run inside the individual Linux challenge containers. Use the SSH connection shown by the course bot for each challenge. Each completed challenge returns a `threadctf{…}` string to submit as text.

- [princess](#princess) · Match zombie children to their parents and signal 50 cursed cabins.
- [princess2](#princess2) · Repeat the process scan in C under the restricted story shell.
- [discodetective](#discodetective) · Handle timed dialogue and combine four verified facts with witness statements.

## princess

The cabins, princesses, and objects in the story are Linux processes. The problem is to identify a cabin with a zombie princess and leave any cabin with a living princess alone. A zombie is a child that has exited and whose parent has yet to collect its exit status. In `ps`, its status begins with `Z`. The child’s PPID gives the PID of the cabin.

Start by inspecting the processes owned by the current user.

```bash
ps -u "$(id -u)" -o pid,ppid,stat,comm
```

For example, the initial snapshot showed `princess_wraith` at PID 18, in state `Z`, with PPID 9. PID 9 was `the_dead_cabin`. PID 20, a `princess_moth`, was in state `S` and had PPID 10, so cabin 10 had to remain running. The cabin process names ended in `_cabin` or started with `cabin_in_`. Some names appeared more than once, which is why the selection used PID and PPID.

The first attempt to send signals using old PIDs returned `No such process` because the cabin set had already changed. Taking a fresh snapshot on every pass fixed that. The following Bash block combines the scans used during the run. It groups princesses by parent, excludes any parent with a living princess, checks that the parent itself is a cabin, and sends `SIGTERM` to the resulting parent PID. The array records successful sends so a cabin that stays visible briefly is counted once.

```bash
declare -A signaled=()
total=0

while (( total < 50 )); do
  while read -r pid name; do
    [[ ${signaled[$pid]+yes} ]] && continue

    if kill -TERM "$pid" 2>/dev/null; then
      signaled[$pid]=1
      total=$((total + 1))
      printf '%s/50 SIGTERM: %s (PID %s)\n' "$total" "$name" "$pid"
      (( total == 50 )) && break
    fi
  done < <(
    ps -u "$(id -u)" -o pid=,ppid=,stat=,comm= |
      awk '
        $4 ~ /^princess_/ && $3 ~ /^Z/  { dead[$2] = 1 }
        $4 ~ /^princess_/ && $3 !~ /^Z/ { alive[$2] = 1 }
        $1 > 1 && $3 !~ /^Z/ && $4 ~ /_cabin$|^cabin_in_/ { cabin[$1] = $4 }
        END {
          for (pid in dead)
            if (pid in cabin && !(pid in alive))
              print pid, cabin[pid]
        }'
  )

  if (( total < 50 )); then
    sleep 2
  fi
done
```

The container printed `50 cursed cabins lie in ruin` after the final signal. It then placed the reward in the home directory. Use `cat ~/flag.txt` to read the submission string.

## princess2

This version asks for 100 cursed cabins. The SSH login gives a story shell that allows `gcc`, an editor, and compiled programs. Save the C source below as `princess2.c` in the home directory inside the container.

```bash
nano princess2.c
gcc -std=c11 -O2 -Wall -Wextra princess2.c -o princess2
./princess2
./princess2 --apply
```

The first invocation displays a preview. It found nine matching cabins during the run. The `--apply` invocation takes a fresh snapshot every two seconds and prints each successful signal. It stops when it can read the reward, when the count reaches 100 in one run, or when its scan limit is reached.

The Linux files `/proc/<pid>/stat` supply the same name, process state, and parent PID as the first challenge. Field 2 is enclosed in parentheses. The parser finds the closing parenthesis before reading fields 3 and 4, so spaces in the process name do not shift the fields. The scan checks the owner of each process directory. For every candidate it also rereads the cabin and verifies that a matching zombie child still has that cabin as its parent. It uses `pidfd_send_signal` on the open cabin directory so a recycled numeric PID cannot redirect the signal to another process.

The first `--apply` run stopped at 96 locally counted sends after its scan limit. Running it again in the same container reset the program’s counter and kept the game’s score. Four more sends produced `100 cursed cabins lie in ruin`, and the program printed `Reward in flag.txt:` followed by the submission string. That message and the file confirmed completion.

### C source

```c
#define _GNU_SOURCE

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

enum { TARGET = 100, ROUNDS = 450, INTERVAL = 2 };

struct process {
    int pid;
    int ppid;
    char state;
    char name[16];
    int cabin_fd;
};

static int is_princess(const char *name)
{
    return strncmp(name, "princess_", 9) == 0;
}

static int is_cabin(const char *name)
{
    size_t length = strlen(name);
    return strncmp(name, "cabin_in_", 9) == 0 ||
           (length >= 6 && strcmp(name + length - 6, "_cabin") == 0);
}

static int read_process(int fd, struct process *process)
{
    struct stat owner;
    if (fstat(fd, &owner) < 0 || owner.st_uid != getuid())
        return 0;

    int stat_fd = openat(fd, "stat", O_RDONLY | O_CLOEXEC);
    if (stat_fd < 0)
        return 0;
    char line[1024];
    ssize_t length = read(stat_fd, line, sizeof(line) - 1);
    close(stat_fd);
    if (length <= 0)
        return 0;
    line[length] = '\0';

    const char *name = strchr(line, '(');
    const char *end = strrchr(line, ')');
    if (name == NULL || end == NULL || end <= name ||
        (size_t)(end - name - 1) >= sizeof(process->name))
        return 0;

    size_t name_length = (size_t)(end - name - 1);
    memcpy(process->name, name + 1, name_length);
    process->name[name_length] = '\0';
    return sscanf(end + 1, " %c %d", &process->state, &process->ppid) == 2;
}

static int zombie_child(const struct process *list, size_t count, int pid)
{
    int zombie = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i].ppid != pid || !is_princess(list[i].name))
            continue;
        if (list[i].state != 'Z')
            return 0;
        zombie = list[i].pid;
    }
    return zombie;
}

static int still_zombie(int proc_fd, int pid, int parent_pid)
{
    char name[32];
    snprintf(name, sizeof(name), "%d", pid);
    int fd = openat(proc_fd, name, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (fd < 0)
        return 0;
    struct process child;
    int matches = read_process(fd, &child) && child.ppid == parent_pid &&
                  child.state == 'Z' && is_princess(child.name);
    close(fd);
    return matches;
}

static int already_sent(const int sent[TARGET], int total, int pid)
{
    for (int i = 0; i < total; i++)
        if (sent[i] == pid)
            return 1;
    return 0;
}

static int scan(int apply, int sent[TARGET], int *total)
{
    DIR *proc = opendir("/proc");
    if (proc == NULL) {
        perror("/proc");
        return -1;
    }

    struct process *list = NULL;
    size_t count = 0, capacity = 0;
    int candidates = 0;
    int proc_fd = dirfd(proc);
    for (;;) {
        errno = 0;
        struct dirent *entry = readdir(proc);
        if (entry == NULL) {
            if (errno != 0) {
                perror("readdir /proc");
                candidates = -1;
            }
            break;
        }
        char *end;
        long pid = strtol(entry->d_name, &end, 10);
        if (*end || pid <= 1 || pid > INT_MAX)
            continue;

        int fd = openat(proc_fd, entry->d_name, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
        if (fd < 0) {
            if (errno == ENOENT || errno == ESRCH || errno == EACCES)
                continue;
            perror("open process directory");
            candidates = -1;
            break;
        }
        struct process process = { .pid = (int)pid, .cabin_fd = -1 };
        if (!read_process(fd, &process)) {
            close(fd);
            continue;
        }
        if (process.state != 'Z' && is_cabin(process.name)) {
            process.cabin_fd = fd;
        } else {
            close(fd);
            if (!is_princess(process.name))
                continue;
        }

        if (count == capacity) {
            size_t next = capacity ? capacity * 2 : 64;
            struct process *grown = realloc(list, next * sizeof(*list));
            if (grown == NULL) {
                perror("realloc");
                if (process.cabin_fd >= 0)
                    close(process.cabin_fd);
                candidates = -1;
                break;
            }
            list = grown;
            capacity = next;
        }
        list[count++] = process;
    }

    for (size_t i = 0; candidates >= 0 && i < count && *total < TARGET; i++) {
        const struct process *cabin = &list[i];
        if (cabin->cabin_fd < 0 || already_sent(sent, *total, cabin->pid))
            continue;

        int zombie = zombie_child(list, count, cabin->pid);
        struct process current;
        if (!zombie || !read_process(cabin->cabin_fd, &current) ||
            current.state == 'Z' || strcmp(current.name, cabin->name) != 0 ||
            !still_zombie(proc_fd, zombie, cabin->pid))
            continue;

        candidates++;
        if (!apply) {
            printf("Candidate: %s (PID %d), zombie child PID %d\n",
                   cabin->name, cabin->pid, zombie);
            continue;
        }

        /* The open /proc/PID directory refers to this cabin even if its PID is reused. */
        if (syscall(SYS_pidfd_send_signal, cabin->cabin_fd, SIGTERM, NULL, 0) == 0) {
            sent[*total] = cabin->pid;
            ++*total;
            printf("%d/%d SIGTERM: %s (PID %d)\n", *total, TARGET, cabin->name, cabin->pid);
        } else if (errno != ESRCH) {
            perror("pidfd_send_signal");
            candidates = -1;
        }
    }

    for (size_t i = 0; i < count; i++)
        if (list[i].cabin_fd >= 0)
            close(list[i].cabin_fd);
    free(list);
    closedir(proc);
    return candidates;
}

static int show_reward(void)
{
    FILE *file = fopen("flag.txt", "r");
    if (file == NULL)
        return 0;

    char line[512];
    int ready = fgets(line, sizeof(line), file) != NULL && line[0] != '\n';
    fclose(file);
    if (ready) {
        printf("Reward in flag.txt:\n%s", line);
        if (strchr(line, '\n') == NULL)
            putchar('\n');
    }
    return ready;
}

int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IOLBF, 0);
    int sent[TARGET];
    int total = 0;
    if (argc == 1) {
        int candidates = scan(0, sent, &total);
        if (candidates >= 0)
            printf("Candidates in this scan: %d\n", candidates);
        return candidates < 0;
    }
    if (argc != 2 || strcmp(argv[1], "--apply") != 0) {
        fprintf(stderr, "Usage: %s [--apply]\n", argv[0]);
        return 2;
    }

    puts("Waiting for cursed cabins and the reward...");
    for (int round = 0; round < ROUNDS && total < TARGET; round++) {
        if (show_reward())
            return 0;
        if (scan(1, sent, &total) < 0)
            return 1;
        sleep(INTERVAL);
    }
    for (int attempt = 0; attempt < 5; attempt++) {
        if (show_reward())
            return 0;
        sleep(INTERVAL);
    }
    printf("Sent %d/%d signals in this run. Reward file was not found.\n", total, TARGET);
    return 1;
}
```

## discodetective

The game is a TCP text service on `127.0.0.1:9999` inside the challenge container. Connecting with `nc 127.0.0.1 9999` shows the dialogue and rules. Ordinary questions use numbered options. A timed thought requires a reply in the form `WARD <sigil> <verb> <formed-urge>`. The first window was three seconds, and later windows dropped to 2.5 seconds. The Python client below handles timed replies and forwards the investigator’s keyboard input.

Save the Python source as `disco_client.py` in the challenge home directory and run it.

```bash
nano disco_client.py
python3 disco_client.py
```

The client buffers partial TCP lines and takes the sigil, verb, form, and action from `!!THOUGHT-STRIKE`. It waits for `!!WARD-WINDOW`, then sends a reply for each strike it collected. A MASKED thought can have different flavors in the attachment and strike lines. During testing, one thought was attached as COMPULSION but struck as EUPHORIA, so the strike required `REFUSE`. The client lowercases the action before forming the words, reverses word order for `reversed`, and uppercases for `shout`. In the successful run, the quoted action “give HELM-9 a hug” became `give-helm-9-a-hug`. Each accepted ward appears as `!!WARD-OK`.

### Collecting evidence

Each new connection can generate different suspects, dialogue numbers, skills, and room order. Read `!!SHEET` and `!!ROUTE` before choosing. For a verified answer without `PAY`, the relevant skill plus five must meet the question’s difficulty. The die controls the result band. A `MESSY` answer at or above the skill threshold can still give a verified fact.

A single line of evidence can leave two suspects. The successful run combined **WHAT**, which checks the project assignment, with **LIE**, which compares the witness statements to the badge records. The badge log gives times by number. The badge map links those numbers to names. Asking for the statements uses up a room, so this run collected both badge records first.

| Room | Choice in this run | Result |
| --- | --- | --- |
| War room | `2`, ask about the checkout core dump | `f1`, ticket `FF-5496` was filed under `Q4 flavors` |
| NOC | `2`, ask who was still in the building | `f2`, the door controller’s last exit time for each badge |
| Tech lead | `6`, ask who was working on what | `f3`, Omar Haddad and Yuki Arakawa worked on `Q4 flavors` |
| Kitchen | `1`, ask about the launch pizza order | `f4`, the pizza order connects badge numbers to names |
| Release manager | `4`, ask for written statements | The four suspects’ claimed departure times |

Omar’s badge `2758` logged an exit at `03:24`, 28 minutes after the `02:56` time he gave. Greta’s badge `5375` logged an exit at `03:23`, 21 minutes after her claimed `03:02`. Ines and Yuki gave times consistent with their badge logs. The game treats a claim more than five minutes before the actual exit as a lie. Omar and Greta were the two whose claims met that rule. Of those two, only Omar was assigned to `Q4 flavors`.

In the review room the state showed `statements=1` and `closable=1`. After the pending ward replies were accepted, send the following lines in order. Wait for `!!PROOF-DEMAND` after the accusation.

```text
ACCUSE 2
PROOF f1 f2 f3 f4
```

The earned statements are already in the case file. The proof cites the verified ticket, badge log, staffing list, and badge map. The game accepted the proof, explained the result, and returned a `!!FLAG` line.

### Recovering a stalled connection

A fresh SSH login returned the same challenge container while an earlier `python3 disco_client.py` was still running on another terminal. The new client connected to TCP port 9999 but received no banner. Checking the current user’s process list found the old client. After confirming its command and PID, stopping that client freed the game to open a fresh case.

```bash
ps -u "$(id -u)" -o pid,ppid,stat,tty,etime,args
ps -p 63 -o pid,tty,args
kill -TERM 63
python3 disco_client.py
```

PID 63 was the previously started Python client in this run. Use the actual PID from the displayed process list if this happens again.

### Python source

```python
import os
import re
import select
import shlex
import socket
import sys
import time

VERBS = {
    "COMPULSION": "FORBID",
    "PARANOIA": "REBUKE",
    "MORTIFICATION": "DISMISS",
    "EUPHORIA": "REFUSE",
    "MALAISE": "DEFY",
}


def make_ward(line):
    fields = dict(
        token.split("=", 1) for token in shlex.split(line)[1:] if "=" in token
    )
    sigil = fields["sigil"]
    verb = VERBS[fields["flavor"]]
    if not re.fullmatch(r"[A-Za-z0-9]+", sigil) or fields.get("verb", verb) != verb:
        raise ValueError("unexpected sigil or verb")

    words = re.findall(r"[a-z0-9]+", fields["action"].lower())
    form = fields["form"]
    if form == "reversed":
        words.reverse()
    elif form == "shout":
        words = [word.upper() for word in words]
    elif form != "kebab":
        raise ValueError(f"unexpected form: {form}")
    if not words:
        raise ValueError("empty action")
    return f"WARD {sigil} {verb} {'-'.join(words)}"


def play():
    print("[client] Connecting to 127.0.0.1:9999...", flush=True)
    with socket.create_connection(("127.0.0.1", 9999), timeout=5) as game:
        game.settimeout(None)
        print("[client] Connected. Waiting for the game banner...", flush=True)
        deadline = time.monotonic() + 5
        keyboard = sys.stdin.fileno()
        inputs = [game, keyboard]
        buffer = b""
        pending = []

        while True:
            wait = None if deadline is None else deadline - time.monotonic()
            if wait is not None and wait <= 0:
                raise TimeoutError("The game sent no banner within 5 seconds.")
            ready, _, _ = select.select(inputs, [], [], wait)

            if game in ready:
                chunk = game.recv(4096)
                if not chunk:
                    print("[client] Game closed the connection.", flush=True)
                    return

                lines = (buffer + chunk).split(b"\n")
                buffer = lines.pop()
                for raw in lines:
                    line = raw.decode("utf-8", errors="replace").rstrip("\r")
                    if line.strip():
                        deadline = None
                    print(line, flush=True)

                    if line.startswith("!!THOUGHT-STRIKE "):
                        try:
                            pending.append(make_ward(line))
                        except (KeyError, ValueError) as error:
                            print(f"[client] Could not form WARD: {error}", flush=True)
                    elif line.startswith("!!WARD-WINDOW ") and pending:
                        game.sendall(("\n".join(pending) + "\n").encode("utf-8"))
                        for command in pending:
                            print(f"[auto] {command}", flush=True)
                        pending.clear()

            if keyboard in ready:
                typed = os.read(keyboard, 4096)
                if typed:
                    game.sendall(typed)
                else:
                    inputs.remove(keyboard)


if __name__ == "__main__":
    try:
        play()
    except KeyboardInterrupt:
        pass
    except OSError as error:
        sys.exit(f"[client] {error}")
```
