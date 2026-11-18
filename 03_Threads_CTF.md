# Threads CTF: 3 accepted challenges

**Instance:** `findingnemo` (3/6 accepted).

Send `/challenge NAME` to the bot before each task. Each SSH challenge uses its **own container**. Connect using the current password from the bot. Submit text flags to InnoFlagranteBot.

## 1. needle

The provided `Meeting.c` shows that `meeting` starts a thread called `retrieval`, which reads `/root/flag.txt`. It then loads `./libmeeting.so` and calls a function named `meeting()` from it.

1. Connect:
   ```bash
   ssh a.khaliullova-needle@osctf.nnk.moe
   ```
2. Create the source for that library:
   ```bash
   nano hack.c
   ```
   Paste:
   ```c
   #include <pthread.h>
   #include <stdio.h>
   #include <stdlib.h>
   
   extern pthread_t retrieval;
   
   void* meeting(void* arg) {
       void* flag;
   
       pthread_join(retrieval, &flag);
       printf("%s\n", (char*)flag);
       fflush(stdout);
       exit(0);
   }
   ```
3. Compile and run:
   ```bash
   gcc -shared -fPIC -o libmeeting.so hack.c -lpthread
   ./meeting
   ```
4. Submit the printed text flag.

**Why:** `pthread_join()` waits for the retrieval thread and gives us its result: the flag's address. We print it and call `exit(0)` before the normal meeting ends.

## 2. princess2

Destroy **100 cabins with dead princesses**. Do not destroy cabins with living princesses. This SSH container has no normal shell: it allows `nano`, `gcc`, and compiled programs.

1. Connect:
   ```bash
   ssh a.khaliullova-princess2@osctf.nnk.moe
   ```
2. Make the program:
   ```bash
   nano hunt.c
   ```
   Paste this full code:
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <dirent.h>
   #include <signal.h>
   #include <unistd.h>
   
   int main() {
       int count = 0;
   
       for (int t = 0; t < 1000000; t++) {
           DIR *dir = opendir("/proc");
           if (!dir) return 1;
   
           struct dirent *entry;
   
           while ((entry = readdir(dir))) {
               int pid = atoi(entry->d_name);
               if (pid <= 0) continue;
   
               char path[64], name[128], state;
               int id, parent;
   
               sprintf(path, "/proc/%d/stat", pid);
               FILE *f = fopen(path, "r");
               if (!f) continue;
   
               int ok = fscanf(f, "%d (%127[^)]) %c %d",
                               &id, name, &state, &parent);
               fclose(f);
   
               if (ok != 4) continue;
               if (state != 'Z') continue;
               if (strncmp(name, "princess_", 9) != 0) continue;
               if (parent <= 1) continue;
   
               sprintf(path, "/proc/%d/stat", parent);
               f = fopen(path, "r");
               if (!f) continue;
   
               char cabin[128], cabin_state;
               int cabin_id, grandparent;
   
               ok = fscanf(f, "%d (%127[^)]) %c %d",
                           &cabin_id, cabin, &cabin_state, &grandparent);
               fclose(f);
   
               if (ok != 4) continue;
               if (cabin_state == 'Z') continue;
               if (!strstr(cabin, "cabin")) continue;
   
               if (kill(parent, SIGTERM) == 0) {
                   count++;
                   printf("Destroyed: %d\n", count);
                   fflush(stdout);
               }
           }
   
           closedir(dir);
           if (count >= 100) break;
           usleep(100000);
       }
   
       printf("Finished: %d\n", count);
       return 0;
   }
   ```
3. Compile and run:
   ```bash
   gcc hunt.c -o hunt
   ./hunt
   ```
4. The server eventually prints:
   ```text
   The forest falls silent. 100 cursed cabins lie in ruin, and
   the story is finally over. Your reward waits in your home
   directory.
   ```
5. Open the flag with the **allowed editor**:
   ```bash
   nano flag.txt
   ```
   Copy the text and submit it to the bot.

**Why:** `/proc/PID/stat` gives a process state and parent PID. `Z` means a dead (zombie) princess. The program checks that her parent is a running cabin and sends `SIGTERM` to the cabin, not to the princess. It repeats because new cabins appear.

**Note:** The server counts cabins across runs. The local `Destroyed` count can be below 100 when the server announces success.

## 3. princess

Destroy **50 cabins with dead princesses**, without touching living ones. This container has a normal Bash shell.

1. Connect:
   ```bash
   ssh a.khaliullova-princess@osctf.nnk.moe
   ```
2. See the process states:
   ```bash
   ps -eo pid,ppid,stat,comm | grep princess
   ```
   `Z` means dead. `PPID` is the cabin's process ID.
3. Run this command:
   ```bash
   n=0
   while [ "$n" -lt 50 ]; do
     while read -r p; do
       if kill "$p" 2>/dev/null; then
         n=$((n+1))
         echo "Destroyed: $n"
       fi
     done < <(ps -eo pid,ppid,stat,comm | awk '
       NR>1 {
         cabin[$1]=($3!="Z" && $4~/cabin/)
         if ($3=="Z" && $4~/^princess_/ && $2>1) dead[$2]=1
       }
       END {for(p in dead) if(cabin[p]) print p}
     ')
     sleep 0.1
   done
   ```
4. When the server announces success, press **Ctrl+C** if the loop is still running. Read the reward:
   ```bash
   ls -la ~
   cat ~/flag.txt
   ```
   Submit the text flag.

**Why:** `ps` lists processes. `awk` selects cabins whose princess is a zombie (`Z`), then `kill` stops those cabins. The loop repeats because their PIDs change. The server counts successful destruction across attempts.
