# dogctf


## PoweredDog

The home directory name `go_to_opt` points to `/opt`. The README gives the full path.

```bash
cat /opt/README.md
# /regular_paw_storage/1374d9441aaa1f23/powered_dog.png
```

## HistoryDog

Search the saved Bash history for `regular_paw_storage`. In the matching `cp` command, the destination is the path to the image.

```bash
grep -F regular_paw_storage ~/.bash_history
# Destination: /regular_paw_storage/60779c71e75e6297/grandpaw.png
```

## CompileDog

Build and run the C program to get the path.

```bash
gcc Programs/compile_dog.c -o /tmp/compile_dog && /tmp/compile_dog
# /regular_paw_storage/9c0b139ccf1ae94a/Compile_dog.png
```

## CatDog

The two files are byte fragments of one PNG. Write `Cat_dog_top` first and `Cat_dog_bottom` second.

```bash
cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > /tmp/catdog.png
file /tmp/catdog.png  # PNG, 223 x 300
```

## TailheadedDog

Join the two path fragments from the beginning and end of the log.

```bash
head Downloads/dog.log        # /regular_paw_storage/7f1fc2d8
tail -n 15 Downloads/dog.log  # 329d99a9/tailheaded.png
# /regular_paw_storage/7f1fc2d8329d99a9/tailheaded.png
```

## BackgroundDog

Resume `background_dog` with `SIGCONT` to get the path.

```bash
pid=$(pgrep -n background_dog)
kill -CONT "$pid"
# /regular_paw_storage/0d6f93b0acc6d18c/Background_dog.png
```

## EditDog

`Doguments/EDITME` holds a "longdog" with the task: make the dog exactly 8 characters longer
without breaking it.
After edition (for example, using nano), the path is printed.

```
regular_paw_storage/9bdf63bd1a741871/Edit_dog.png
```

## MuteDog

Run it once with stdout piped to `cat`, then with both stdout and stderr piped to `cat`. Join the two path fragments.

```bash
./Programs/mute_dog | cat       # /regular_paw_storage/e83e13b1
./Programs/mute_dog 2>&1 | cat  # Look for 929c38ab/Mute_dog.png
# /regular_paw_storage/e83e13b1929c38ab/Mute_dog.png
```

## ScatteredDog

Join the fragments in numeric order, from `-50` to `49`.

```bash
cd "Videos/Scattered dog"
for i in $(seq -50 49); do
  cat "./$i"
done > /tmp/scattered.png
file /tmp/scattered.png  # PNG, 300 x 195
```

## ShyDog

Use `file` to find the PNG among the `*_Maybe_Dog_*` files.

```bash
cd "Videos/Shy dog"
for f in *_Maybe_Dog_*; do
  if file -b -- "$f" | grep -q '^PNG'; then
    printf '%s\n' "$f"
  fi
done
# PIC_Maybe_Dog_61553489.png (150 x 200)
```

## InsecureDog

We can check a specific filename in `super_secret/` even though we can’t list the directory.

```Doguments/secret_stuff/remember.txt ``` says the name before .png is four letters long and uses only D, O, G.
```
cd Doguments/secret_stuff
for a in D O G; do for b in D O G; do for c in D O G; do for d in D O G; do
  f="super_secret/$a$b$c$d.png"; [ -f "$f" ] && echo "$f"
done; done; done; done
# -> super_secret/DODG.png   (PNG, 300 x 225)
```

## PetDog

The current files in `frames/` are stubs. Git history contains the three blobs used to reconstruct `mascot.png`. Extract them in part order.

```bash
cd Doguments/auto_petter
git cat-file blob 221288240e97 > /tmp/p1
git cat-file blob 82ef5bcd787d > /tmp/p2
git cat-file blob dfd574e9c801 > /tmp/p3
cat /tmp/p1 /tmp/p2 /tmp/p3 > /tmp/mascot.png
file /tmp/mascot.png  # PNG, 300 x 200
```
---

## cctf

### cavern

cavern has ```fenc``` and ```fkey``` arrays, which are global and const, also it has ```offering_set_secret``` function, which does not check ```this``` pointer. It means, that ```offering_set_secret``` can be called from library and the secret will be correct.

```c
#include <stdio.h>
#include <dlfcn.h>
char buffer [1024];
void run(){
        void (*set_secret)(void*) = dlsym(RTLD_DEFAULT, "offering_set_secret");
        set_secret(buffer);
        for (int i = 0; i<1024; i++){
                if (buffer[i]!=0) putchar(buffer[i]);
        }
        putchar('\n');
}
```

### legacy

The code causes segfault, because ```position``` hash function was used on 16-bit architecture and it uses integer overflow to cut the ```h```. The solution is to change ```unsigned h``` in ```position``` function to ```unsigned short h```.

### xorcism

The password check compares input byte by byte against `enc[i] ^ key[i % 4]`, with a 4-byte key
and a 33-byte `enc` table both present in the source. XOR the table back with the key to recover
the password:

```c
#include <stdio.h>

int main(void) {
    unsigned char enc[33] = {
        0x67,0x4d,0x69,0x3b,0x44,0x12,0x05,0x4b,0x58,0x4f,0x2e,
        0x50,0x51,0x12,0x69,0x6b,0x68,0x16,0x12,0x6a,0x68,0x4f,
        0x3b,0x3a,0x03,0x6d,0x05,0x4b,0x72,0x4c,0x35,0x41,0x44,
    };
    unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };
    for (int i = 0; i < 33; i++) putchar(enc[i] ^ key[i % 4]);
    putchar('\n');
    return 0;
}
```

---

## threadctf

### decrypt

The decryptor does two things to slow the execution: calls ```sleep(10, 0)``` and uses random function to ```exit(1)``` sometimes. I changed ```sleep``` and ```jnz``` after random function to NOPs and used decryptor to all sharps:

``` for i in $(ls shards | sort -V); do ./decrapinator-x86_64-linux decrypt "shards/$i"; done 2>/dev/null; echo ```
### needle

The binary has permission to read root. So, library just uses it to read from root directory.

```c
#include <stdio.h>

void *meeting(void *arg) {
    FILE *f = fopen("/root/flag.txt", "r");
    char c;
    while ((c = fgetc(f)) != EOF) putchar(c);
    return 0;
}
```

```
gcc -shared -fPIC -o libmeeting.so name.c
./meeting
```

### princess

Each cabin runs its princess as a child process named `princess_*`. When the princess dies she
becomes a zombie that the cabin never reaps, so a cursed cabin keeps a dead `princess_*` child.
For every process the program reads `/proc/<pid>/stat`, which holds the name, state, and parent
pid. When it finds a zombie named `princess_*`, it kills the parent pid, which is that cursed cabin.
It repeats until the forest writes the flag to `/home/ctf/flag.txt`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

int main(void) {
    for (;;) {
        DIR *d = opendir("/proc");
        struct dirent *e;
        while (d && (e = readdir(d))) {
            if (!isdigit((unsigned char)e->d_name[0]))
                continue;

            char path[300], buf[512];
            snprintf(path, sizeof path, "/proc/%s/stat", e->d_name);
            FILE *f = fopen(path, "r");
            if (!f)
                continue;
            int n = fread(buf, 1, sizeof buf - 1, f);
            fclose(f);
            buf[n > 0 ? n : 0] = 0;

            /* stat is "pid (comm) state ppid ...": name after '(', state and ppid after last ')' */
            char *open = strchr(buf, '(');
            char *close = strrchr(buf, ')');
            if (!open || !close)
                continue;
            char state = 0;
            int ppid = 0;
            sscanf(close + 2, "%c %d", &state, &ppid);

            /* a dead princess is a zombie named princess_*; her parent is the cursed cabin */
            if (state == 'Z' && ppid > 1 && strncmp(open + 1, "princess_", 9) == 0)
                kill(ppid, SIGKILL);
        }
        if (d)
            closedir(d);

        FILE *f = fopen("/home/ctf/flag.txt", "r");
        if (f) {
            char line[256];
            int ok = fgets(line, sizeof line, f) && strstr(line, "threadctf");
            fclose(f);
            if (ok) {
                fputs(line, stdout);
                return 0;
            }
        }
        usleep(300000);   /* 0.3s between scans */
    }
}
```

```
gcc princess.c -o k && ./k        # prints the flag once the forest drops it
```

### princess2

Same challenge and same program. The flag still appears at `/home/ctf/flag.txt`. The only
difference is the shell. Here the forest replaced the login shell with a restricted "story-shell"
that runs only `gcc`, `nano`, `vim`, `rm`, `cat`, and programs you compiled (`./k`). It has no
pipes and no output redirection, so you cannot build the source file with `cat >` or pipe it into
the compiler. Open `nano`, type the princess program into it, then build and run:

```
nano -w k.c        # type in the princess program above
gcc k.c -o k
./k                # prints the flag
```

Pass `nano -w` so nano does not wrap the long lines. Wrapping splits a line in the middle and
breaks the code.
