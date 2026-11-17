## Princess - Accepted

**Goal:** Find and destroy 50 cursed cabins containing dead princesses (zombie processes) without killing any living princesses.

### Steps

1. Connected to the challenge container using SSH:


   After entering the password, I accessed the Linux container.

2. Inspected the running processes to understand their structure:

   ```bash
   ps -eo pid,ppid,stat,comm | head -n 30
   ```

   The command displayed the process ID (`PID`), parent process ID (`PPID`), process state (`STAT`), and command name (`COMMAND`).

   I noticed several processes named `princess_*` and `*_cabin`.

   Some princesses had the state `Z` (zombie), while others had `S` (sleeping).

   A zombie process has already terminated, but its parent has not collected its exit status using `wait()` or `waitpid()`.

3. Filtered the process list to find only dead princesses:

   ```bash
   ps -eo pid,ppid,stat,comm | awk '$3 ~ /^Z/ && $4 ~ /^princess_/'
   ```

   The `awk` command selected processes whose state started with `Z` and whose names started with `princess_`.

   Example output:

   ```text
   PID  PPID STAT COMMAND
   17      9 Z    princess_heart
   34     15 Z    princess_burned
   36     14 Z    princess_burned
   40     18 Z    princess_hollow
   ```

   The `PPID` column identified the parent process of each dead princess.

4. Examined the process hierarchy:

   ```bash
   ps -eo pid,ppid,stat,comm --forest | head -n 80
   ```

   The `--forest` option displayed parent-child relationships as a process tree.

   For example:

   ```text
   175    1 S  the_fog_cabin
   176  175 Z   \_ princess_den
   177  175 S   \_ the_coin
   178  175 S   \_ the_blade
   179  175 S   \_ the_veil
   ```

   This showed that the cabin was the parent process of the princess.

   Since `princess_den` was already a zombie, its parent cabin was a valid target.

   I also found cabins containing living princesses (`S`), which had to remain untouched.

5. Attempted to terminate a cabin containing a dead princess:

   ```bash
   ps -eo pid,ppid,stat,comm | awk '$3 ~ /^Z/ && $4 ~ /^princess_/ {print $2; exit}'
   ```

   The command returned the parent PID of the first zombie princess.

   I tried:

   ```bash
   kill 215
   ```

   However, the process had already disappeared:

   ```text
   -bash: kill: (215) - No such process
   ```

   This happened because the challenge continuously recreated its processes, causing their PIDs to change.

   I needed to reduce the delay between identifying a target and sending the termination signal.

6. Combined process identification and termination into one command:

   ```bash
   ps -eo pid,ppid,stat,comm --no-headers |
   awk '$3 ~ /^Z/ && $4 ~ /^princess_/ {print $2; exit}' |
   xargs -r kill
   ```

   The command used `awk` to find the parent PID and `xargs` to pass it directly to `kill`.

   I checked the exit code:

   ```bash
   echo $?
   ```

   Output:

   ```text
   0
   ```

   This confirmed that the pipeline completed successfully, although it did not independently confirm the challenge's progress.

7. Created a filter to identify cabins containing zombie princesses while excluding cabins with living princesses:

   ```bash
   ps -eo pid,ppid,stat,comm --no-headers | awk '
   $4 ~ /cabin/ { cabins[$1] = $4 }
   $4 ~ /^princess_/ {
       if ($3 ~ /^Z/) dead[$2] = 1
       else alive[$2] = 1
   }
   END {
       for (p in dead)
           if (cabins[p] && !alive[p])
               print p, cabins[p]
   }'
   ```

   I used three associative arrays:

   - `cabins` stored cabin process IDs.
   - `dead` stored parent PIDs of zombie princesses.
   - `alive` stored parent PIDs of living princesses.

   The condition `cabins[p] && !alive[p]` ensured that only identified cabins without living princesses were selected.

   Example output:

   ```text
   359 the_last_cabin
   406 cabin_in_pines
   384 the_far_cabin
   424 the_lake_cabin
   380 the_fog_cabin
   397 the_lake_cabin
   343 the_quiet_cabin
   363 the_twin_cabin
   ```

8. Used a Bash loop to terminate the selected cabins:

   ```bash
   ps -eo pid,ppid,stat,comm --no-headers | awk '
   $4 ~ /cabin/ { cabins[$1] = 1 }
   $4 ~ /^princess_/ {
       if ($3 ~ /^Z/) dead[$2] = 1
       else alive[$2] = 1
   }
   END {
       for (p in dead)
           if (cabins[p] && !alive[p])
               print p
   }' | while read -r pid; do
       if kill "$pid" 2>/dev/null; then
           echo "Destroyed cabin PID: $pid"
       fi
   done
   ```

   The script searched for valid cabins and sent them the default `SIGTERM` signal using `kill`.

   Output:

   ```text
   Destroyed cabin PID: 462
   Destroyed cabin PID: 481
   Destroyed cabin PID: 424
   Destroyed cabin PID: 397
   Destroyed cabin PID: 467
   Destroyed cabin PID: 448
   ```

   This confirmed that six termination signals were successfully sent.

9. Automated the process to repeat until 50 successful `kill` commands were executed:

   ```bash
   count=0

   while (( count < 50 )); do
       while read -r pid; do
           # Check that the parent is still a cabin
           [[ $(ps -p "$pid" -o comm= 2>/dev/null) == *cabin* ]] || continue

           if kill "$pid" 2>/dev/null; then
               ((count+=1))
               echo "Cabin killed: $pid ($count/50)"
           fi

           (( count >= 50 )) && break

       done < <(
           ps -eo pid,ppid,stat,comm --no-headers | awk '
           $4 ~ /cabin/ { cabins[$1] = 1 }
           $4 ~ /^princess_/ {
               if ($3 ~ /^Z/) dead[$2] = 1
               else alive[$2] = 1
           }
           END {
               for (p in dead)
                   if (cabins[p] && !alive[p])
                       print p
           }'
       )

       sleep 0.1
   done
   ```

   The outer `while` loop repeated the search, while the inner loop processed matching cabin PIDs.

   The script also checked that each PID still belonged to a cabin before sending the signal.

   Since the challenge continuously recreated processes, `sleep 0.1` introduced a short delay between iterations.

   The script displayed its progress:

   ```text
   Cabin killed: 533 (1/50)
   Cabin killed: 506 (2/50)
   Cabin killed: 566 (3/50)
   ...
   Cabin killed: 9488 (40/50)
   Cabin killed: 9724 (41/50)
   Cabin killed: 10075 (42/50)
   ```

   The challenge then displayed:

   ```text
   === The Forest ===
   The forest falls silent. 50 cursed cabins lie in ruin, and
   the story is finally over. Your reward waits in your home
   directory.
   ```

   Although my script had counted 42 successful signals during this run, the challenge confirmed that 50 cursed cabins had been destroyed in total.

10. Located the flag in the home directory:

    ```bash
    cd ~
    ls -la
    ```

    The directory contained:

    ```text
    -rw-r--r-- 1 root root 50 Nov 17 18:41 flag.txt
    ```

    I read the file:

    ```bash
    cat flag.txt
    ```

    Output:

    ```text
    threadctf{WHat_do_yOU_MeAn_TheY_Are_41r3adY_deaD}
    ```

**Result:** Successfully destroyed 50 cursed cabins without intentionally targeting living princesses. The challenge confirmed completion, and I retrieved the flag from `~/flag.txt`.

**Flag:** `threadctf{WHat_do_yOU_MeAn_TheY_Are_41r3adY_deaD}`

**Challenge completed.**


## Princess2 — Accepted

**Category:** C Language / Linux Processes  
**Challenge:** `princess2`  
**Goal:** Destroy 100 cursed cabins containing dead princesses (zombie processes) without killing living princesses.

### Steps

#### 1. Connected to the challenge container

I connected to the challenge container using SSH:



```

After entering the password, I received the following instructions:

```text
The forest kept your shell. It won't give it back.
All you have left is gcc and whatever you compile.

Allowed: gcc, vim/nano, rm and ./binaries
Forbidden: Everything else
```

Unlike the previous `princess` challenge, normal Linux commands such as `ps`, `cat`, `ls`, and `kill` were not available.

I had to implement process inspection and termination using the C programming language.

#### 2. Tested access to Linux processes

Linux provides process information through the `/proc` virtual filesystem.

I created `inspect.c` to check whether I could read this information.

```bash
nano inspect.c
```

**Complete code — `inspect.c`:**

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("/proc/1/comm", "r");

    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char name[128];

    if (fgets(name, sizeof(name), file)) {
        printf("PID 1: %s", name);
    }

    fclose(file);
    return 0;
}
```

I compiled and executed the program:

```bash
gcc inspect.c -o inspect
./inspect
```

Output:

```text
PID 1: the_forest
```

The program used `fopen()` to open `/proc/1/comm`, `fgets()` to read the process name, and `fclose()` to close the file.

This confirmed that I could inspect Linux processes directly through C.

#### 3. Found princess processes and their parent cabins

I created `find_princess.c` to scan `/proc` and find all princess processes.

```bash
nano find_princess.c
```

The program needed to identify four important fields:

- `PID` — process identifier.
- `PPID` — parent process identifier.
- `STATE` — process state.
- `NAME` — process name.

I also used the parent PID to retrieve the name of the corresponding cabin.

**Complete final code — `find_princess.c`:**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

int main(void) {
    DIR *proc = opendir("/proc");

    if (proc == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit((unsigned char)entry->d_name[0]))
            continue;

        char path[256];

        snprintf(path, sizeof(path),
                 "/proc/%s/status", entry->d_name);

        FILE *file = fopen(path, "r");

        if (file == NULL)
            continue;

        char line[256];
        char name[128] = "";
        char state = '?';
        int ppid = -1;

        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "Name: %127s", name);
            sscanf(line, "State: %c", &state);
            sscanf(line, "PPid: %d", &ppid);
        }

        fclose(file);

        // Find all princess processes
        if (strncmp(name, "princess_", 9) == 0 && ppid > 1) {
            char parent_path[256];
            char parent_name[128];

            snprintf(parent_path, sizeof(parent_path),
                     "/proc/%d/comm", ppid);

            FILE *parent_file = fopen(parent_path, "r");

            if (parent_file == NULL)
                continue;

            if (fgets(parent_name, sizeof(parent_name), parent_file)) {
                parent_name[strcspn(parent_name, "\n")] = '\0';

                printf("PID=%s | State=%c | Parent PID=%d | Parent=%s\n",
                       entry->d_name, state, ppid, parent_name);
            }

            fclose(parent_file);
        }
    }

    closedir(proc);
    return 0;
}
```

I compiled and executed it:

```bash
gcc find_princess.c -o find_princess
./find_princess
```

Output included:

```text
PID=505 | State=Z | Parent PID=504 | Parent=the_cabin
PID=513 | State=Z | Parent PID=512 | Parent=the_last_cabin
PID=518 | State=Z | Parent PID=517 | Parent=the_witch_cabin
PID=522 | State=S | Parent PID=521 | Parent=the_far_cabin
PID=537 | State=Z | Parent PID=536 | Parent=the_lake_cabin
```

I discovered that princess processes had two important states:

- `S` — sleeping process, representing a living princess.
- `Z` — zombie process, representing a dead princess.

A zombie process has already terminated, but its parent has not collected its exit status using `wait()` or `waitpid()`.

Therefore, I needed to terminate the parent cabin rather than the zombie process itself.

For example:

```text
PID 536 - the_lake_cabin
    |
    +-- PID 537 - princess (Z)
```

The parent cabin was a potential target because its princess was already dead.

The program used `opendir()` and `readdir()` to enumerate processes, `sscanf()` to extract fields from `/proc/<PID>/status`, and `strncmp()` to identify princesses.

#### 4. Implemented safe cabin detection and tested one termination

Before automating the process, I needed to ensure that cabins containing living princesses would not be targeted.

I created `slay_one.c`:

```bash
nano slay_one.c
```

The main idea was to find a zombie princess, identify its parent cabin, verify its child processes, and send `SIGTERM` only when no living princesses were found.

**Complete code — `slay_one.c`:**

```c
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>

typedef struct {
    int pid;
    int ppid;
    char state;
    char name[128];
} Process;

// Read process information from /proc
int read_process(int pid, Process *p) {
    char path[64];
    char line[256];

    snprintf(path, sizeof(path),
             "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");
    if (!file)
        return 0;

    p->pid = pid;
    p->ppid = -1;
    p->state = '?';
    p->name[0] = '\0';

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Name:", 5) == 0)
            sscanf(line + 5, "%127s", p->name);

        if (strncmp(line, "State:", 6) == 0)
            sscanf(line + 6, " %c", &p->state);

        if (strncmp(line, "PPid:", 5) == 0)
            sscanf(line + 5, "%d", &p->ppid);
    }

    fclose(file);

    return p->ppid >= 0 && p->state != '?'
           && p->name[0] != '\0';
}

// Check whether a cabin is safe to destroy
int safe_cabin(int cabin_pid) {
    Process parent;

    if (!read_process(cabin_pid, &parent))
        return 0;

    if (!strstr(parent.name, "cabin") || parent.ppid != 1)
        return 0;

    DIR *dir = opendir("/proc");
    if (!dir)
        return 0;

    struct dirent *entry;
    int dead = 0;
    int alive = 0;

    while ((entry = readdir(dir)) != NULL) {
        char extra;
        int pid;

        if (sscanf(entry->d_name, "%d%c", &pid, &extra) != 1)
            continue;

        Process child;

        if (!read_process(pid, &child))
            continue;

        if (child.ppid != cabin_pid)
            continue;

        if (strncmp(child.name, "princess_", 9) != 0)
            continue;

        if (child.state == 'Z')
            dead++;
        else
            alive++;
    }

    closedir(dir);

    return dead > 0 && alive == 0;
}

int main(void) {
    DIR *dir = opendir("/proc");

    if (!dir) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        char extra;
        int pid;

        if (sscanf(entry->d_name, "%d%c", &pid, &extra) != 1)
            continue;

        Process princess;

        if (!read_process(pid, &princess))
            continue;

        if (strncmp(princess.name, "princess_", 9) != 0)
            continue;

        if (princess.state != 'Z')
            continue;

        int cabin_pid = princess.ppid;

        // Verify the cabin and its princesses
        if (!safe_cabin(cabin_pid))
            continue;

        // Recheck before sending SIGTERM
        Process current;

        if (!read_process(pid, &current))
            continue;

        if (current.state != 'Z' ||
            current.ppid != cabin_pid)
            continue;

        if (!safe_cabin(cabin_pid))
            continue;

        if (kill(cabin_pid, SIGTERM) == 0) {
            printf("Killed cabin PID=%d\n", cabin_pid);
            closedir(dir);
            return 0;
        }
    }

    closedir(dir);
    printf("No suitable cabin found\n");
    return 0;
}
```

I compiled the program:

```bash
gcc -Wall -Wextra slay_one.c -o slay_one
```

Then I executed it:

```bash
./slay_one
```

Output:

```text
Killed cabin PID=536
```

This confirmed that the program successfully sent a termination signal to one selected cabin.

**How the safety check worked:**

The `safe_cabin()` function checked that the parent was a cabin and then inspected its child processes.

The important condition was:

```c
return dead > 0 && alive == 0;
```

This required at least one dead princess and zero identified living princesses.

I also rechecked the selected process before sending the signal because the challenge continuously recreated processes and their PIDs could change.

These checks reduced the chance of targeting the wrong process, although they could not completely eliminate race conditions.

#### 5. Automated the process to destroy 100 cabins

After successfully testing one termination, I created a complete automated program.

```bash
nano slay_all.c
```

The program reused the same process inspection and safety-checking logic but added a loop to repeat the operation.

**Complete code — `slay_all.c`:**

```c
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int pid;
    int ppid;
    char state;
    char name[128];
} Process;

// Read information about a process
int read_process(int pid, Process *p) {
    char path[64];
    char line[256];

    snprintf(path, sizeof(path),
             "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");
    if (!file)
        return 0;

    p->pid = pid;
    p->ppid = -1;
    p->state = '?';
    p->name[0] = '\0';

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Name:", 5) == 0)
            sscanf(line + 5, "%127s", p->name);

        if (strncmp(line, "State:", 6) == 0)
            sscanf(line + 6, " %c", &p->state);

        if (strncmp(line, "PPid:", 5) == 0)
            sscanf(line + 6, "%d", &p->ppid);
    }

    fclose(file);

    return p->ppid >= 0 &&
           p->state != '?' &&
           p->name[0] != '\0';
}

// Check whether a cabin is safe to destroy
int safe_cabin(int cabin_pid) {
    Process parent;

    if (!read_process(cabin_pid, &parent))
        return 0;

    if (!strstr(parent.name, "cabin") ||
        parent.ppid != 1)
        return 0;

    DIR *dir = opendir("/proc");
    if (!dir)
        return 0;

    struct dirent *entry;
    int dead = 0;
    int alive = 0;

    while ((entry = readdir(dir)) != NULL) {
        char extra;
        int pid;

        if (sscanf(entry->d_name, "%d%c",
                   &pid, &extra) != 1)
            continue;

        Process child;

        if (!read_process(pid, &child))
            continue;

        if (child.ppid != cabin_pid)
            continue;

        if (strncmp(child.name, "princess_", 9) != 0)
            continue;

        if (child.state == 'Z')
            dead++;
        else
            alive++;
    }

    closedir(dir);

    return dead > 0 && alive == 0;
}

// Find and destroy one suitable cabin
int slay_one(void) {
    DIR *dir = opendir("/proc");

    if (!dir) {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        char extra;
        int pid;

        if (sscanf(entry->d_name, "%d%c",
                   &pid, &extra) != 1)
            continue;

        Process princess;

        if (!read_process(pid, &princess))
            continue;

        if (strncmp(princess.name, "princess_", 9) != 0)
            continue;

        if (princess.state != 'Z')
            continue;

        int cabin_pid = princess.ppid;

        if (!safe_cabin(cabin_pid))
            continue;

        // Recheck princess and cabin
        Process current;

        if (!read_process(pid, &current))
            continue;

        if (current.state != 'Z' ||
            current.ppid != cabin_pid)
            continue;

        if (!safe_cabin(cabin_pid))
            continue;

        if (kill(cabin_pid, SIGTERM) == 0) {
            printf("Killed cabin PID=%d\n", cabin_pid);
            closedir(dir);
            return cabin_pid;
        }
    }

    closedir(dir);
    return 0;
}

// Main loop
int main(void) {
    int count = 0;

    struct timespec delay = {
        .tv_sec = 0,
        .tv_nsec = 100000000L
    };

    while (count < 150 &&
           access("/home/ctf/flag.txt", F_OK) != 0) {

        int pid = slay_one();

        if (pid < 0) {
            fprintf(stderr, "Process scan failed\n");
            return 1;
        }

        if (pid > 0) {
            count++;
            printf("Signals sent: %d | Target: 100\n",
                   count);
            fflush(stdout);
        }

        nanosleep(&delay, NULL);
    }

    if (access("/home/ctf/flag.txt", F_OK) == 0) {
        printf("\nChallenge reward detected!\n");
    } else {
        printf("\nStopped after %d signals.\n", count);
    }

    return 0;
}
```

**Explanation of the main functions:**

| Function | Purpose |
|---|---|
| `read_process()` | Reads a process's name, state, PID, and PPID from `/proc`. |
| `safe_cabin()` | Checks whether a cabin contains zombie princesses and no living princesses. |
| `slay_one()` | Finds a suitable cabin and attempts to terminate it using `kill()`. |
| `main()` | Repeats the operation and checks for the reward file. |

The program used:

```c
kill(cabin_pid, SIGTERM);
```

to request the termination of selected cabin processes.

It also used:

```c
nanosleep(&delay, NULL);
```

to introduce a 100-millisecond delay between iterations.

The following condition checked whether the reward file existed:

```c
access("/home/ctf/flag.txt", F_OK)
```

The loop continued until the reward file appeared or 150 successful termination signals had been sent.

The limit of 150 was a safeguard, while the actual challenge objective was to destroy 100 cabins.

#### 6. Compiled and executed the automated program

I compiled the final program:

```bash
gcc -Wall -Wextra slay_all.c -o slay_all
```

The options `-Wall` and `-Wextra` enabled additional compiler warnings.

I executed the program:

```bash
./slay_all
```

Beginning of the output:

```text
Killed cabin PID=2119
Signals sent: 1 | Target: 100
Killed cabin PID=2132
Signals sent: 2 | Target: 100
Killed cabin PID=2137
Signals sent: 3 | Target: 100
Killed cabin PID=2141
Signals sent: 4 | Target: 100
Killed cabin PID=2145
Signals sent: 5 | Target: 100
```

The program continued finding and terminating suitable cabins.

At the end, I received:

```text
Killed cabin PID=3086
Signals sent: 96 | Target: 100
Killed cabin PID=3090
Signals sent: 97 | Target: 100
Killed cabin PID=3094
Signals sent: 98 | Target: 100
Killed cabin PID=3121
Signals sent: 99 | Target: 100

=== The Forest ===
The forest falls silent. 100 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.

Challenge reward detected!
```

The challenge confirmed that **100 cursed cabins had been destroyed**.

The automated program counted 99 successful signals. Before running it, I had also successfully terminated one cabin while testing `slay_one.c`, which explains the difference if that earlier termination was included in the challenge's counter.

The program then detected the reward file.

#### 7. Retrieved the flag using C

Since normal shell commands such as `cat` were forbidden, I created another C program to read the reward file.

```bash
nano read_flag.c
```

**Complete code — `read_flag.c`:**

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("/home/ctf/flag.txt", "r");

    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}
```

I compiled and executed it:

```bash
gcc -Wall -Wextra read_flag.c -o read_flag
./read_flag
```

Output:

```text
threadctf{I_hOpE_YOu_CaN_C_clearly_Now}
```

The program used `fopen()` to open `/home/ctf/flag.txt`, `fgets()` to read its contents, and `printf()` to display the flag.

### Result

I successfully completed the `princess2` challenge using C without relying on normal Linux shell commands.

I inspected processes through `/proc`, identified zombie princesses, located their parent cabins, and implemented checks to avoid targeting living princesses.

I then automated process termination using `kill()` and `nanosleep()`.

The challenge confirmed that 100 cursed cabins had been destroyed, and I retrieved the flag using a separate C program.

**Flag:** `threadctf{I_hOpE_YOu_CaN_C_clearly_Now}`

**Challenge completed.**




## DiscoDetective — Accepted

**Category:** Python / Networking / CTF  
**Challenge:** `discodetective`  
**Goal:** Investigate the Flag Factory incident, identify the employee responsible for the unauthorized deployment, and prove the accusation using verified evidence.

### Steps

#### 1. Connected to the challenge container

I connected to the challenge container using SSH:

```bash
ssh o.korchagin-discodetective@osctf.nnk.moe
```

After entering the password, I successfully accessed the Linux container.

The challenge description explained that the Flag Factory had been sabotaged. Someone had deployed a change that removed the lock from the stock counter, allowing the shop to sell more flags than were available.

My task was to interview employees, collect evidence, identify the responsible person, and prove the accusation.

The game was running on a local TCP server:

```text
127.0.0.1:9999
```

I initially connected using Netcat:

```bash
nc 127.0.0.1 9999
```

The server displayed the game introduction, instructions, suspects, and available questions.

The important instructions included:

```text
!!MANUAL ward_format="WARD <sigil> <verb> <formed-urge>" window_ms=3000
!!MANUAL proof_format="PROOF <fact-id> [<fact-id> ...]"
!!MANUAL mind="carrying 5 thoughts at once is fatal" limit=5
```

The game required me to answer intrusive thoughts within a short time limit, normally around three seconds.

During my first manual connection, I failed to answer an intrusive thought in time.

Output:

```text
!!THOUGHT-STRIKE sigil=b15e type=IMPULSE
flavor=EUPHORIA verb=REFUSE form=kebab
action="announce a new flavor"

!!WARD-WINDOW ms=3000 count=1

You didn't answer it in time, so it answered for you.

!!GAME-OVER reason=thought-won
```

This showed that manually entering every `WARD` command would be unreliable.

I decided to write a Python client that would automatically handle intrusive thoughts while allowing me to choose investigation options manually.

#### 2. Analyzed the game protocol

The game used a line-based text protocol over TCP.

It provided several important message types:

| Message | Purpose |
|---|---|
| `!!SHEET` | Character skill values |
| `!!OPTIONS` | Available investigation questions |
| `!!PICK` | Request for an option number |
| `!!THOUGHT-STRIKE` | Information needed to counter an intrusive thought |
| `!!WARD-WINDOW` | Time window for submitting WARD commands |
| `!!WARD-OK` | Confirmation that a thought was countered |
| `!!FACT` | Evidence discovered during an investigation |
| `!!UNLOCK` | Additional investigation question unlocked |
| `!!CASE` | Current investigation status |
| `!!PROMPT accuse` | Request to identify the suspect |
| `!!PROOF-DEMAND` | Request for supporting evidence |
| `!!FLAG` | Successful challenge completion |

The most important protocol format was:

```text
WARD <sigil> <verb> <formed-urge>
```

For example:

```text
!!THOUGHT-STRIKE sigil=3f22
type=IMPULSE flavor=PARANOIA
verb=REBUKE form=kebab
action="tape over every webcam in the office"
```

The correct response was:

```text
WARD 3f22 REBUKE tape-over-every-webcam-in-the-office
```

The `sigil`, `verb`, `form`, and `action` fields were provided by the server.

The game supported three different formats for intrusive thoughts:

- `kebab` — lowercase words separated by hyphens.
- `reversed` — words reversed and separated by hyphens.
- `shout` — uppercase words separated by hyphens.

I needed to parse these fields and automatically generate the correct response before the deadline.

#### 3. Created an automated Python client

I created a Python file inside the container:

```bash
nano detective.py
```

The program used Python's standard libraries, so no additional packages were required.

I used `socket` for TCP communication, `threading` to receive messages while accepting user input, `shlex` to parse structured fields, and `re` to format intrusive thoughts.

**Complete code — `detective.py`:**

```python
import socket
import threading
import shlex
import re


def form_urge(action, form):
    words = re.findall(r"[A-Za-z0-9]+", action)

    if form == "reversed":
        words.reverse()

    result = "-".join(word.lower() for word in words)

    if form == "shout":
        result = result.upper()

    return result


def listen(sock):
    pending = []

    try:
        stream = sock.makefile(
            "r", encoding="utf-8", errors="replace"
        )

        for raw in stream:
            print(raw, end="", flush=True)
            line = raw.strip()

            if line.startswith("!!THOUGHT-STRIKE "):
                parts = shlex.split(line.split(" ", 1)[1])

                fields = dict(
                    item.split("=", 1)
                    for item in parts
                    if "=" in item
                )

                sigil = fields["sigil"]
                verb = fields["verb"]
                action = fields["action"]
                form = fields["form"]

                urge = form_urge(action, form)

                ward = f"WARD {sigil} {verb} {urge}"
                pending.append(ward)

            if line.startswith("!!WARD-WINDOW "):
                for ward in pending:
                    sock.sendall((ward + "\n").encode())
                    print(f"\n[AUTO] {ward}", flush=True)

                pending.clear()

    except (OSError, KeyError, ValueError) as error:
        print(f"\nError: {error}")

    print("\n[Connection closed]")


def main():
    with socket.create_connection(
        ("127.0.0.1", 9999)
    ) as sock:

        thread = threading.Thread(
            target=listen,
            args=(sock,),
            daemon=True
        )

        thread.start()

        try:
            while thread.is_alive():
                command = input()
                sock.sendall((command + "\n").encode())

        except (KeyboardInterrupt, EOFError, OSError):
            pass


if __name__ == "__main__":
    main()
```

**Explanation of the program:**

The `form_urge()` function converted intrusive thought text into the format requested by the server.

For example:

```python
form_urge("announce a new flavor", "kebab")
```

Produced:

```text
announce-a-new-flavor
```

The `listen()` function continuously received server messages.

Whenever a line started with `!!THOUGHT-STRIKE`, the program extracted the required response fields.

For example:

```python
sigil = fields["sigil"]
verb = fields["verb"]
action = fields["action"]
form = fields["form"]
```

It then generated a WARD command:

```python
ward = f"WARD {sigil} {verb} {urge}"
```

The program stored pending commands until the server announced:

```text
!!WARD-WINDOW
```

It then sent the commands automatically:

```python
sock.sendall((ward + "\n").encode())
```

The `threading.Thread` allowed the program to receive server messages while the main thread accepted my investigation choices through `input()`.

This meant that I could manually choose questions without needing to respond to intrusive thoughts myself.

#### 4. Tested the Python client

I launched the program:

```bash
python3 detective.py
```

The client successfully connected to the game server.

During the investigation, the server generated an intrusive thought:

```text
!!THOUGHT-STRIKE sigil=3f22
type=IMPULSE flavor=PARANOIA
verb=REBUKE form=kebab
action="tape over every webcam in the office"

!!WARD-WINDOW ms=3000 count=1
```

My program automatically responded:

```text
[AUTO] WARD 3f22 REBUKE tape-over-every-webcam-in-the-office

!!WARD-OK sigil=3f22
```

This confirmed that the automatic WARD system worked.

The client was also able to handle several thoughts in the same window.

Example:

```text
!!WARD-WINDOW ms=2750 count=2

[AUTO] WARD 3f22 REBUKE tape-over-every-webcam-in-the-office
[AUTO] WARD 5fe7 REFUSE order-pizza-for-the-whole-building-on-the-company-card

!!WARD-OK sigil=3f22
!!WARD-OK sigil=5fe7
```

Later, it also handled `MASKED` thoughts correctly by using the information from `!!THOUGHT-STRIKE` instead of relying on the original thought description.

The automated client solved the time-sensitive part of the challenge, allowing me to focus on the investigation.

#### 5. Investigated the case in the first Python attempt

In the first attempt using the Python client, the game generated four suspects:

1. Marta Kowal — product manager
2. Kofi Mensah — intern
3. Nikolai Brandt — night NOC
4. Felix Ostrowski — staff engineer

I investigated the available rooms by selecting questions.

In the war room, I selected option `4`, which investigated the deployment time.

I received:

```text
[verified f1] The deploy log:
the change that broke checkout went out at 02:44.

!!FACT id=f1 kind=deploy-time
provenance=verified at=02:44
```

At the tech lead's desk, I selected option `3` to obtain staffing information.

```text
!!FACT id=f2 kind=staffing provenance=verified
```

The employees were assigned to two workstreams:

- Search tuning: Felix Ostrowski and Nikolai Brandt.
- Mobile app: Kofi Mensah and Marta Kowal.

In the kitchen, I selected option `1` and obtained the night rotation's written statements.

At the NOC, I selected option `3` and obtained the badge log:

```text
!!FACT id=f3 kind=badge-log provenance=verified
entries="2954=03:01;5354=03:07;3792=01:53;6510=02:23;3642=03:06;5172=02:20"
```

In the release manager's room, I selected option `1` and found the deleted ticket:

```text
!!FACT id=f4 kind=change
provenance=verified
stream="mobile app"
ticket=FF-1428
```

However, I did not obtain the badge map needed to connect badge numbers with employees.

At the review room, the game reported:

```text
!!CASE verified=badge-log,change,deploy-time,staffing
hearsay= statements=1 rooms_solved=4 closable=0
```

The value `closable=0` meant that I did not have enough evidence to prove one suspect's guilt.

I ended the attempt using:

```text
WALK
```

Output:

```text
You close the file and go home.

!!GAME-OVER reason=walked
```

This first attempt helped me understand which types of evidence were necessary and how the investigation system worked.

#### 6. Started a new investigation

I restarted the Python client:

```bash
python3 detective.py
```

The game generated a new scenario with different skill values and suspects.

The new skills were:

```text
!!SHEET LOGIC=2 EMPATHY=2 DRAMA=2 VOLITION=1
INLAND_EMPIRE=6 PHYSICAL=5 RHETORIC=4 SUGGESTION=3
```

The new suspects were:

1. Hana Novak — night SRE
2. Marta Kowal — backend engineer
3. Sasha Petrenko — tech lead
4. Aiko Mori — security engineer

I needed to collect verified evidence and identify which suspect could have deployed the change.

#### 7. Found the malicious change in the war room

In the first room, I selected option `3`:

```text
3) [PHYSICAL: Routine 9]
"Pull the core dump from the checkout service."
```

My `PHYSICAL` skill was `5`.

The game used the rule:

```text
skill + 5 >= difficulty
```

Since `5 + 5 >= 9`, I could pass the check.

I entered:

```text
3
```

Result:

```text
!!RESULT die=5 band=CLEAN n=3 margin=1

[verified f1] The core dump:
the comment says 'FF-1731: drop the lock, per Memo 7'.
FF-1731 is a payments cleanup ticket.

!!FACT id=f1 kind=change
provenance=verified stream="payments cleanup"
ticket=FF-1731
```

This gave me the first verified fact.

**Evidence f1:** The malicious change was filed under the `payments cleanup` workstream.

#### 8. Found the staffing information

In the tech lead's room, I selected option `2`:

```text
2) [EMPATHY: Routine 7]
"Who are you protecting?"
```

I entered:

```text
2
```

Result:

```text
!!RESULT die=3 band=MESSY n=2 margin=-2

[verified f2] The sprint board for tonight.
On payments cleanup: Aiko Mori and Hana Novak.
On performance: Sasha Petrenko and Marta Kowal.

!!FACT id=f2 kind=staffing provenance=verified
```

This produced the second verified fact.

**Evidence f2:** Only Hana Novak and Aiko Mori were working on `payments cleanup`.

Therefore, I could eliminate Marta Kowal and Sasha Petrenko based on their assigned workstreams.

The game also unlocked the ability to obtain witness statements:

```text
!!STATEMENTS available=1 rooms_solved=2
```

#### 9. Investigated the release channel

In the release channel, I first selected option `5`:

```text
5) [RHETORIC: Medium 11]
"Ask who has been messaging them privately tonight."
```

My `RHETORIC` skill was `4`, meaning I was two points short.

The game displayed:

```text
!!SHORT n=5 diff=11 skill=RHETORIC
short=2 cost=2thoughts payable=1

!!PROMPT short hint="send: PAY or ACCEPT"
```

There were two possible responses:

- `PAY` — force the question at the cost of two intrusive thoughts.
- `ACCEPT` — accept secondhand information without verification.

I selected:

```text
ACCEPT
```

Result:

```text
!!ACCEPTED n=5
!!UNLOCK scene=founder provenance=hearsay
!!FAILS scene=channel count=1 limit=2
```

This unlocked an additional question in the founder's office.

However, the lead was based on hearsay, not verified evidence.

The game allowed me to ask another question in the same room.

I then selected option `1`:

```text
1) [INLAND_EMPIRE: Routine 9]
"Ask for the night rotation's written statements."
```

I entered:

```text
1
```

Result:

```text
!!RESULT die=9 band=CRIT n=1 margin=6

It takes some asking, and one phone call to someone
who was asleep, but a printout of all four statements
ends up in your hands.

!!STATEMENTS earned=1 rooms_solved=2
```

I obtained the following statements:

| Suspect | Claimed departure time |
|---|---|
| Hana Novak | 02:17 |
| Marta Kowal | 03:35 |
| Sasha Petrenko | 01:12 |
| Aiko Mori | 02:23 |

The server registered them as claims:

```text
!!CLAIM id=c1 suspect=1 kind=left-at at=02:17
!!CLAIM id=c2 suspect=2 kind=left-at at=03:35
!!CLAIM id=c3 suspect=3 kind=left-at at=01:12
!!CLAIM id=c4 suspect=4 kind=left-at at=02:23
```

These statements were important, but I still needed the real departure times to determine who was lying.

#### 10. Unlocked additional verified evidence

In the founder's office, I selected option `3`:

```text
3) [INLAND_EMPIRE: Medium 10]
"Read the sticky notes around the founder's monitor."
```

My `INLAND_EMPIRE` skill was `6`, so I could pass the check.

I entered:

```text
3
```

Result:

```text
!!RESULT die=10 band=CRIT n=3 margin=6

!!UNLOCK scene=oncall provenance=verified
!!CALM sigil=4d29
!!MIND count=1 limit=5
```

The critical success unlocked a verified question in the next room.

It also removed one intrusive thought through the `CALM` effect.

The unlocked question was especially useful because the game could provide two facts from one successful check.

#### 11. Obtained the badge log and badge map

In the NOC room, I selected option `3`:

```text
3) [PHYSICAL: Routine 9]
"Ask to see the door log with names attached,
the one you heard about."
```

I entered:

```text
3
```

Result:

```text
!!RESULT die=4 band=CLEAN n=3 margin=0
```

The game provided two verified facts.

**Evidence f3 — Badge Log**

```text
!!FACT id=f3 kind=badge-log provenance=verified
entries="6650=01:52;9982=02:11;5882=03:17;8571=02:23;8989=03:09;3378=03:35"
```

This contained the last departure time recorded for each badge.

**Evidence f4 — Badge Map**

```text
!!FACT id=f4 kind=badge-map provenance=verified
entries="8989=Dmitri Sokol (release manager);8571=Aiko Mori (security engineer);6650=Sasha Petrenko (tech lead);5882=Hana Novak (night SRE);9982=Greta Holm (night NOC);3378=Marta Kowal (backend engineer)"
```

This mapped badge numbers to employee names.

I combined the information:

| Employee | Badge | Real departure |
|---|---|---|
| Hana Novak | 5882 | 03:17 |
| Marta Kowal | 3378 | 03:35 |
| Sasha Petrenko | 6650 | 01:52 |
| Aiko Mori | 8571 | 02:23 |

Now I could compare the employees' statements with the actual security records.

#### 12. Identified the culprit

At the review room, I received:

```text
!!CASE verified=badge-log,badge-map,change,staffing
hearsay= statements=1 rooms_solved=3 closable=1
```

The value `closable=1` confirmed that I had enough evidence to close the investigation.

I compared the statements against the verified badge log:

| Suspect | Claimed departure | Actual departure | Difference |
|---|---|---|---|
| Hana Novak | 02:17 | 03:17 | 60 minutes |
| Marta Kowal | 03:35 | 03:35 | 0 minutes |
| Sasha Petrenko | 01:12 | 01:52 | 40 minutes |
| Aiko Mori | 02:23 | 02:23 | 0 minutes |

Hana Novak and Sasha Petrenko both gave incorrect departure times.

However, the workstream evidence showed:

```text
Payments cleanup:
- Hana Novak
- Aiko Mori

Performance:
- Sasha Petrenko
- Marta Kowal
```

Only Hana Novak satisfied both important conditions:

1. She worked on the `payments cleanup` ticket that contained the malicious change.
2. She lied about her departure time, claiming to have left one hour earlier.

Aiko Mori worked on the same ticket but had a departure time matching the security records.

Sasha Petrenko also gave an incorrect departure time, but worked on a different workstream.

Therefore, **Hana Novak was the only suspect matching all the verified evidence**.

#### 13. Accused the suspect

The game displayed:

```text
!!PROMPT accuse hint="send: ACCUSE <n>"
```

Hana Novak was suspect number `1`.

I entered:

```text
ACCUSE 1
```

The server responded:

```text
!!PROOF-DEMAND format="PROOF <fact-id> [<fact-id> ...]"

You turn to Hana Novak (night SRE) and say it out loud.
They ask, quite reasonably, what makes you think so.
```

The game required me to provide evidence supporting the accusation.

#### 14. Submitted the final proof

I used the four verified facts collected during the successful investigation.

| Fact ID | Evidence | Description |
|---|---|---|
| `f1` | Change | The malicious ticket was part of payments cleanup |
| `f2` | Staffing | Hana Novak worked on payments cleanup |
| `f3` | Badge log | Badge 5882 left at 03:17 |
| `f4` | Badge map | Badge 5882 belonged to Hana Novak |

I entered:

```text
PROOF f1 f2 f3 f4
```

The server accepted the evidence.

Output:

```text
You lay it out for the room.

The change was a payments cleanup ticket,
and they were on payments cleanup tonight.

They say they left at 02:17.
The door log has them leaving at 03:17.

Nobody else on the rotation fits.
```

Hana Novak admitted that the founder had instructed them to prevent checkout from waiting, and they removed the stock counter lock.

The game confirmed the successful investigation:

```text
HELM-9: PRODUCTION HEALTHY.

!!FLAG threadctf{put_0n_panTs_N0T_imp0st3R_syndr0M3}

Your flag: threadctf{put_0n_panTs_N0T_imp0st3R_syndr0M3}

[Connection closed]
```

### Result

I successfully completed the `discodetective` challenge by developing a Python TCP client and investigating the Flag Factory incident.

The client automated responses to time-sensitive intrusive thoughts by parsing structured server messages and generating `WARD` commands.

I manually investigated the available rooms, collected verified facts, compared witness statements with security records, and identified Hana Novak as the employee responsible for the unauthorized deployment.

I then submitted the accusation with four verified facts, and the game accepted my proof.

**Culprit:** Hana Novak (night SRE)

**Final commands:**

```text
ACCUSE 1
PROOF f1 f2 f3 f4
```

**Flag:** `threadctf{put_0n_panTs_N0T_imp0st3R_syndr0M3}`

**Challenge completed.**

