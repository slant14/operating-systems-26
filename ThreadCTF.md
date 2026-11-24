i.birkina@innopolis.university CSE-02

# ThreadCTF

## 1. Needle

I was given `Meeting.c` and a letter explaining how to compile a shared library.

First, I looked at the source code. I found that the program created two threads. The first thread, called retrieval, read the flag from `/root/flag.txt`. The second thread executed my function `meeting()` from the library `libmeeting.so`.

I noticed that the main program used `pthread_join()` to wait for the retrieval thread and get its result.

I decided to get this result before the main program.

First, I connected to the server and checked the files.

**Commands:**

```bash
pwd
ls -la
```

I found the executable file `meeting`.

Then I created my own C file:

```bash
nano program_with_name.c
```

Inside it, I wrote a `meeting()` function that used `dlsym()` to find the retrieval thread:

```c
pthread_t *retrieval = dlsym(RTLD_DEFAULT, "retrieval");
```

After that, I used `pthread_join()` to get the result of this thread:

```c
void *fabric = NULL;
pthread_join(*retrieval, &fabric);
printf("FLAG: %s\n", (char *)fabric);
```

This allowed my program to get the flag before the main program could receive it.

Then I compiled my code into a shared library and executed the meeting.

**Commands:**

```bash
gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread -ldl
./meeting
```

The program printed:

```text
FLAG: threadctf{shoO7Ing_sOMe_aTOmIc_fEet}

The fabric is nowhere to be found!
Polythread is very angry.

Thankfully, your name is still with you.
```

## 2. Princess

In this task, I needed to destroy 50 cursed cabins without touching the living princesses.

First, I connected to the server and checked the files.

**Commands:**

```bash
pwd
ls -la
find . -maxdepth 2 -print | head -n 60
ls -la /
```

I did not find any files related to the cabins, so I decided to check the running processes.

**Commands:**

```bash
ps aux | head -n 30
ps -eo pid,ppid,stat,comm,args | head -n 30
```

I noticed that the cabins and princesses were represented by Linux processes.

Some princesses had the state Z, which means Zombie. These processes had already finished, so the princesses were dead.

For example:

```text
268 the_twin_cabin princess_mirror
269 Z princess_mirror <defunct>

273 the_cabin princess_den
274 Z princess_den <defunct>
```

I understood that I needed to terminate the parent processes of dead princesses.

To find them, I used:

```bash
ps -eo ppid=,stat=,comm= | awk '$2 ~ /^Z/ && $3 ~ /^princess_/ {print $1}'
```

This command found dead princesses and printed the PID of their parent processes.

Then I checked the names of these processes:

```bash
ps -eo ppid=,stat=,comm= | awk '$2 ~ /^Z/ && $3 ~ /^princess_/ {print $1}' | sort -nu | while read pid; do ps -p "$pid" -o pid=,comm=,args=; done
```

I confirmed that their parents were cabins.

Next, I wrote a shell loop that found these cabins and terminated them using kill.

My first version processed only one cabin at a time. It was slow, so after removing 21 cabins, I changed the loop to process all available dead cabins in each iteration.

The important part was:

```bash
if ps -p "$pid" -o comm= | grep -q 'cabin'; then
    if kill "$pid" 2>/dev/null; then
        count=$((count + 1))
        echo "Progress: $count/50"
    fi
fi
```

This checked that the parent process was a cabin before sending a termination signal.

I repeated this until the server printed:

```text
=== The Forest ===

The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.
```

This confirmed that I had completed the task.

I retrieved the flag from my home directory.

## 3. Fort Forks

In this task, I needed to read a very large amount of data from a pipe to find the flag.

The main problem was that every process, including my SSH session, was terminated after 5 seconds.

First, I connected to the server and checked its files.

**Commands:**

```bash
pwd
ls -la
ls -la /
ls -l /proc/self/fd
```

I found a file called `secrets` in `/home/ctf`.

Its permissions started with p:

```text
prw-r--r-- 1 root root 0 secrets
```

This meant that it was a named pipe (FIFO).

I could not simply read the whole pipe because my process would be killed after 5 seconds. Also, reopening the pipe would restart the reading.

The task description mentioned eating utensils, so I understood that the solution was related to `fork()`.

I decided to write a C program that would regularly create a new process and continue reading using the same open file descriptor.

First, I created `fortforks.c` on my Mac.

**Command:**

```bash
nano ~/fortforks.c
```

In this program, I opened the pipe:

```c
int fd = open("/home/ctf/secrets", O_RDONLY | O_NONBLOCK);
```

Then I used `fork()` to create a child process:

```c
pid_t child = fork();

if (child < 0) return 1;
if (child > 0) return 0;
```

The child inherited the open pipe and could continue reading the same data stream.

I also added a loop that created another child approximately every second. The old process exited, and the new process continued reading.

This allowed the program to keep working despite the 5-second process limit.

I used `poll()` and `read()` to receive data from the pipe.

The program searched the data for `threadctf{`. When it found the complete flag, it saved it to:

```text
/home/ctf/flag.txt
```

After writing the code, I transferred it from my Mac to the server.

**Command:**

```bash
scp ~/fortforks.c i.birkina-fortforks@osctf.nnk.moe:fortforks.c
```

The transfer completed successfully.

Then I compiled and started the program remotely:

```bash
ssh i.birkina-fortforks@osctf.nnk.moe 'gcc -O2 fortforks.c -o fortforks && ./fortforks'
```

At first, I tried to read the flag, but the file did not exist yet.

I checked whether the program had compiled and whether the flag had appeared:

```bash
ssh i.birkina-fortforks@osctf.nnk.moe 'ls -l ~/fortforks; pgrep -af fortforks; ls -l ~/flag.txt'
```

The output showed that fortforks had compiled successfully and flag.txt had been created.

Finally, I read the result:

```bash
ssh i.birkina-fortforks@osctf.nnk.moe 'cat /home/ctf/flag.txt'
```

The file contained the flag
