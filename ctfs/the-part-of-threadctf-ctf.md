# ThreadCTF

## 1. Needle

The program loads `meeting()` from `libmeeting.so`. The `retrieval` thread reads the flag and returns it as `char *`. I made my own library. It waits for the thread and prints its result.

```c
#include <pthread.h>
#include <stdio.h>

void meeting(pthread_t thread)
{
    void *result;

    pthread_join(thread, &result);
    puts((char *)result);
}
```

```bash
gcc -Wall -Wextra -shared -fPIC meeting.c -o libmeeting.so -pthread
LD_LIBRARY_PATH=. ./meeting
```

**Flag** — `threadctf{shOot1Ng_SOMe_atomIc_fEet}`

## 2. Princess

I found `princess_*` zombie processes.

```bash
ps -eo pid=,ppid=,stat=,comm= | awk '$3 ~ /^Z/ {print}'
```

A zombie is already dead. Its PPID points to a live `*_cabin` parent. I killed only the parent processes.

```bash
ps -p <PPID1>,<PPID2> -o pid,ppid,stat,comm
kill -9 <PPID1> <PPID2>
```

New zombies appeared. After I killed 50 cabins, the flag appeared.

```bash
cat ~/flag.txt
```

**Flag** — `threadctf{WH4t_dO_YoU_meAN_THEY_4R3_4LREaDy_D34d}`

## 3. Princess2

There was no shell. I wrote a C program that read `/proc/<pid>/stat`, found `princess_*` zombies, and sent `SIGKILL` to their parent cabins. I saved each parent PID so I did not count it twice. The program stopped after 100 cabins.

The flag appeared in `/home/ctf/flag.txt`. The `cat` command was also missing, so I printed the file with C.

```c
#include <stdio.h>

int main(void)
{
    FILE *file = fopen("/home/ctf/flag.txt", "r");
    int ch;

    if (file == NULL) {
        return 1;
    }

    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
    return 0;
}
```

```bash
gcc readflag.c -o readflag
./readflag
```

**Flag** — `threadctf{1_hope_You_caN_C_C134Rly_n0w}`
