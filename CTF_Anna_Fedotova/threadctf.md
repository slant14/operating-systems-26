# Threads CTF (threadctf)

## 1. princess

The container has many processes: "cabins" that start "princesses". A dead princess is a zombie process (status `Z`, `<defunct>`): it has already finished, but its parent did not clean it up with `wait()`, so it stays in the process list. A zombie cannot be killed, because it is already dead. To remove it, its parent (the cursed cabin) is killed instead. Then `init` (PID 1, the forest) takes the zombie and cleans it up. The task is to kill the parents of 50 zombies, and not kill the living princesses.

```bash
ps aux | grep defunct     # see the dead princesses (zombies)
# kill the parents of all current zombies; repeat until 50 are done
kill $(ps -eo stat=,ppid= | awk '$1 ~ /Z/ {print $2}')
ls -la                    # flag.txt appears in the home directory
cat flag.txt
```

Flag:

```
threadctf{Wha7_Do_yOu_M3aN_th3Y_Are_a1rEADy_D3aD}
```

## 2. Meeting

The `meeting` program has the setuid root bit (`-rwsr-xr-x`), so it runs as root and is allowed to read `/root/flag.txt`. It starts one thread that reads the flag, and it also loads our `libmeeting.so` and runs our `meeting()` function as a second thread in the same process. Threads in the same process share the same rights, so our `meeting()` also runs as root. It can open `/root/flag.txt` by itself and print it, and it returns `NULL`, so the name is not given away.

`program_with_name.c`:

```c
#include <stdio.h>

void* meeting(void* arg) {
    FILE* f = fopen("/root/flag.txt", "r");
    char buf[256] = {0};
    if (f) {
        fgets(buf, 256, f);
        fclose(f);
    }
    printf("%s\n", buf);
    return NULL;
}
```

```bash
gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread
./meeting
```

Flag:

```
threadctf{5ho0tING_somE_4ToM1C_feeT}
```

## 3. decrypt

The challenge gives 500 encrypted shards. The task text says the decryptor takes 10 seconds for one shard, that it often crashes, and that the whole thing (download, decrypt, submit) must be done in 30 seconds. Doing the shards one after another would take far more than 30 seconds, so the manager runs them all at the same time. For each shard it starts several copies at once, so that even if some copies crash, at least one works. The decryptor's banner and error messages go to stderr, which the code throws away (`/dev/null`); the real flag piece goes to stdout. A crashed copy leaves empty output, so for each shard the manager keeps the first copy whose output is not empty. The pieces are joined in shard-number order.

`manager.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHARDS    500
#define ATTEMPTS  30
#define DECRYPTOR "./Decrapinator.v2.Linux.MacOS.Cracked/decrapinator-x86_64-linux"

int main(void) {
    system("rm -rf out && mkdir out");

    for (int shard = 0; shard < SHARDS; shard++) {
        for (int a = 0; a < ATTEMPTS; a++) {
            if (fork() != 0) continue;
            char in[128], out[128];
            sprintf(in,  "shards/%d.shard", shard);
            sprintf(out, "out/%d-%d", shard, a);
            freopen(out, "w", stdout);
            freopen("/dev/null", "w", stderr);
            execl(DECRYPTOR, DECRYPTOR, "decrypt", in, (char *)NULL);
            _exit(1);
        }
    }

    while (wait(NULL) != -1) ;

    char flag[8192] = "";
    for (int shard = 0; shard < SHARDS; shard++) {
        for (int a = 0; a < ATTEMPTS; a++) {
            char out[128], line[256] = "";
            sprintf(out, "out/%d-%d", shard, a);
            FILE *f = fopen(out, "r");
            if (f == NULL) continue;
            fgets(line, sizeof line, f);
            fclose(f);
            line[strcspn(line, "\r\n")] = '\0';
            if (line[0] != '\0') {
                strcat(flag, line);
                break;
            }
        }
    }

    printf("%s\n", flag);
    return 0;
}
```

```bash
gcc -o manager manager.c
# within the 30s window (decryptor is already downloaded; only shards are new):
tar xzf shards.tar.gz && ./manager
```

Flag:

```
threadctf{You_t0Ta11Y_wrote_A_prOPer_pRogRaM_foR_tHiS_Ri9Ht}
```
