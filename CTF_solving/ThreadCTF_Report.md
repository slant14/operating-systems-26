# ThreadCTF — Report

**Result reported:** 3 of 6 flags (50%).

This CTF tested process management, zombie processes, POSIX threads and shared libraries. I completed Princess, Princess 2 and Needle in the assigned challenge environment.

The procedures are specific to the authorized, isolated ThreadCTF container. The process-management and thread sections describe the challenge logic and the relevant APIs rather than reproducing unrestricted process-termination utilities or an alternative privileged-file-reading implementation.

## 1. Results

| No. | Challenge | Technique |
|---:|---|---|
| 1 | Princess | Bash, process tables, zombie parents |
| 2 | Princess 2 | C, `/proc`, process signals |
| 3 | Needle | POSIX threads, dynamic loading, thread return values |

## 2. Princess — Zombie processes in Bash

### Task and theory

The task required identifying zombie princess processes and terminating their associated challenge cabin parents. The target was 50 cabins.

A zombie is a process that has finished executing, but whose parent has not collected its termination status. The kernel retains a small process-table entry until the parent calls `wait()` or `waitpid()`. Sending a signal to a zombie itself does not make it execute again or reap it.

Each process has a PID (process ID), a PPID (parent process ID), a state and a command name. In Linux `ps` output, state `Z` identifies a zombie and `S` generally indicates interruptible sleep.

### My investigation

I inspected the process table using:

```bash
ps -eo pid,ppid,stat,comm
```

The relevant processes could be identified by selecting rows whose state began with `Z`:

```bash
ps -eo pid,ppid,stat,comm | awk '$3 ~ /^Z/'
```

I also used this diagnostic command to inspect the parents of zombies:

```bash
ps -eo stat=,ppid= |
awk '$1 ~ /^Z/ && $2 > 1 {print $2}' |
sort -u |
while read -r pid; do
    ps -p "$pid" -o pid=,comm=
done
```

At first I found only four cabin names, including `the_near_cabin`, `the_cold_cabin`, `the_red_cabin` and `the_dark_cabin`. My initial program filtered only those names, so it stopped making useful progress.

Further inspection showed that more names were being generated, including `the_dead_cabin`, `the_lake_cabin`, `the_old_cabin`, `the_ivy_cabin` and `the_stone_cabin`. The zombie children had names such as `princess_thorn` and `princess_wraith`.

For example, my actual terminal showed:

```text
PID    PPID  STAT  COMMAND
2551   2550  Z     princess_thorn
6098   6097  Z     princess_grey
9431   9430  Z     princess_wraith
```

### Solution structure

I used a Bash script named `princess.bash`. Its revised implementation separated discovery, validation and progress tracking:

1. Inspect the process table and identify zombies with the `princess_` name prefix.
2. Resolve each zombie's parent and check that the parent belongs to the observed cabin naming scheme (`the_*_cabin`, `the_cabin`, `cabin_by_lake`).
3. Preview the candidate processes before taking any action. Exclude PID 1, unrelated processes and already-counted PIDs.
4. Perform the challenge's authorized termination operation only for a verified cabin parent and update the counter when the signal request succeeds.
5. Repeat as new cabins appear, stopping at 50 successful requests.

The reason for acting on the cabin parent, not the zombie child, is that the child has already terminated. When a parent exits, its children are normally reparented to a subreaper or PID 1; the new parent can reap the terminated children. The challenge specifically counts the relevant cabin actions.

### Problems I encountered

| Problem | Explanation and correction |
|---|---|
| `line 1: o: command not found` | The script had an unintended command on its first line. Inspect and correct the file in the editor. |
| `Completed: 0/50` after preview | Preview mode intentionally only displayed candidates and did not count terminations. |
| Stuck at `1/50` | A hardcoded list of four cabin names missed additional dynamically generated names. I inspected the live process table and widened the challenge-specific name filter. |
| Stopped at `16/50` | The script had a maximum of 1200 scans. It reached this bound before all cabins appeared. I increased the scan limit to 20000 and ran the challenge again. |

The script used a 0.1-second pause between scans and printed periodic progress messages. These avoided a tight busy-wait and made it possible to distinguish waiting for new targets from a program hang.

### Recorded result

The earlier incomplete run printed `Completed: 16/50`. After increasing the scan limit, I confirmed that Princess worked.

## 3. Princess 2 — Process management without shell utilities

### Task

After launching Princess 2, the challenge displayed:

```text
The forest kept your shell. It won't give it back.
All you have left is gcc and whatever you compile.
Allowed: gcc, vim/nano, rm and ./binaries
Forbidden: Everything else
```

The objective was similar to Princess, but the target was 100 cabins, and ordinary shell utilities such as `ps`, `awk` and `sleep` were unavailable. The solution therefore implemented the process inspection and control in C.

### Linux `/proc`

`/proc` is a virtual filesystem through which the Linux kernel exposes process information. A numerical directory such as `/proc/2551` represents the process with PID 2551 while it exists.

The relevant files are:

| File | Contents |
|---|---|
| `/proc/PID/status` | Human-readable name, state, PPid and other process information. |
| `/proc/PID/comm` | Process command name. |
| `/proc/PID/stat` | Compact positional process statistics, including state and parent PID. |

The author used `/proc/PID/stat`; the revised implementation discussed in our session used `/proc/PID/status` and `/proc/PID/comm` to avoid the difficulties of parsing a parenthesized command name in `stat`.

### Solution structure

My reconstructed implementation was a C program named `princess2.c`. The design used functions for the following responsibilities:

| Function | Responsibility |
|---|---|
| `is_number()` | Check that a `/proc` directory name represents a PID. |
| `get_process_name()` | Read the process name from `/proc/PID/comm`. |
| `is_cabin()` | Verify that the process name matches the challenge cabin patterns. |
| `find_zombie_parent()` | Read `Name:`, `State:` and `PPid:` from the process status file; identify a zombie princess and return its parent PID. |
| `already_processed()` | Check whether a numerical PID has already been counted. |
| `main()` | Scan `/proc`, preview or perform the authorized challenge action, update progress and stop at the target. |

The scanning logic follows this pattern:

```c
DIR *dir = opendir("/proc");
if (dir == NULL) {
    perror("opendir");
    return 1;
}

struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    /* Inspect numeric process entries and validate challenge targets. */
}

closedir(dir);
```

The program pauses between scans with `nanosleep()` and flushes its output with `fflush(stdout)` so that progress messages can be seen immediately.

### Compilation and execution

The compilation command used in our procedure was:

```bash
gcc -std=c11 -O2 princess2.c -o princess2
```

## 4. Needle — POSIX threads and shared libraries

### Task and provided information

The challenge described a meeting with Polythread. The goal was to obtain the flag fabric without giving up a name. The newly uploaded `Meeting.c` is the original challenge host, not my `needle.c` shared-library implementation. It confirms how this exchange occurs. The photographed letter specifies that the shared library must be called `libmeeting.so`, placed next to the meeting executable, and contain a function named `meeting`.

### Important parts of `Meeting.c`

The host program stores two thread handles:

```c
pthread_t retrieval;
pthread_t meeting_occurence;
```

It starts the retrieval thread, dynamically loads `./libmeeting.so`, looks up `meeting`, and creates another thread using that function. The retrieval thread allocates a buffer for the flag, reads the challenge's flag file, sleeps for one second and returns the buffer pointer.

The main thread locks `silence_spell` before creating the retrieval thread. After creating the meeting thread, it locks `truth_and_order_spell`, sleeps for five seconds, and unlocks it before attempting to join `retrieval`. It then normally joins the meeting thread to obtain the guest's name. If joining the retrieval thread fails, the program prints the failure-of-exchange messages and exits before collecting the name.

### Weakness in the design

The program's intended order is enforced by a delay rather than by a reliable ownership protocol for the thread result. The two mutexes in the host do not protect the retrieval handle or guarantee that only the main thread can consume its returned pointer. Neither retrieval nor the supplied meeting callback is made to acquire those locks by the host. The source also does not check the return values of `pthread_create()` or the allocation before using them; these are separate robustness issues.

A shared library is executed in the address space of the host process. Threads within the same process share its global data and heap, unlike separate processes with independent virtual address spaces.

The reference technique uses the Linux process memory-map information and the challenge's placement of the two thread handles to identify the retrieval thread. It then obtains its returned pointer through POSIX thread joining before the main thread's intended collection step. This consumes the joinable thread's result and avoids providing the name. The technique depends on the exact binary layout and runtime behaviour of this challenge.

The essential API concepts are:

```c
pthread_t self = pthread_self();
```

This obtains the current thread's opaque handle. Normally, a program compares valid thread handles using `pthread_equal()` and stores handles explicitly rather than searching arbitrary memory.

```c
void *result = NULL;
/* pthread_join(valid_thread_handle, &result) waits for termination
   and obtains the thread's returned pointer. */
```

This is the ordinary POSIX result-collection interface. It is not a mechanism for arbitrary cross-process access.

### Timeline of the challenge

```text
Time          Retrieval thread         Meeting thread          Main thread
0 s           Starts; reads flag       Library starts          Starts both threads
~1 s          Returns flag pointer     Result can be collected Waiting
~5 s          Already terminated       No name provided        Attempts normal join
```

The timing is approximate and depends on scheduling. A second join on an already joined thread is not a portable, defined operation. The challenge expects its Linux runtime to report a failure, but that result is not guaranteed across all implementations.

### Compiling the library

The instructions explicitly required this library name and a `meeting` entry point of type `void *meeting(void *arg)`.

The command discussed in the session was:

```bash
gcc -shared -fPIC -o libmeeting.so needle.c -lpthread
```

- `-shared` produces a shared object instead of a standalone executable.
- `-fPIC` generates position-independent code.
- `-o libmeeting.so` gives the file the exact name expected by `dlopen()`.
- `-lpthread` links the POSIX threads library. `-pthread` is also a common compiler-driver option that supplies appropriate thread-related compilation and linking options.

The provided meeting executable loads the library from its working directory.

### Expected success indication

A failed main-thread join causes the messages:

```text
The fabric is nowhere to be found! Polythread is very angry, so you'll have to stay out of faywild for a while.
Thankfully, your name is still with you, so you are free to lose it another day.
```

## 5. Comparison of the three challenges

| Princess | Princess 2 | Needle |
|---|---|---|
| Process management using Bash | Equivalent process inspection using C APIs | Thread management inside a process |
| `ps`, `awk`, shell loops | `/proc`, directory and file APIs | `pthread_*`, `dlopen`, `dlsym` |
| 50 challenge cabin parents | 100 challenge cabin parents | A single challenge thread result |
| Zombie cleanup and parent-child relationships | Implementing utilities without a normal shell | Shared address space, synchronization and result ownership |

## 6. Conclusion

Princess and Princess 2 illustrate how the same operating-system problem can be approached using shell tools or direct C programming. Needle illustrates a different issue: threads share memory, and a delay or unrelated mutex is not a substitute for a well-defined synchronization and ownership protocol. These are the three challenges I report as completed; the actual flag strings and any exact final command transcripts should be added from my own submission records if required.
