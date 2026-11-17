# Legacy

**C Language CTF (`cctf`) · Instance `findingcheburashka` · Challenge `legacy`**

## legacy

The task was to fix an old C program that caused a segmentation fault on a modern computer. The original developer's machine used an 18-bit `unsigned` integer, while my computer uses 32 bits.

The problem was in the `position()` function, which calculates a hash using `h * 67 + *s++`. This hash is then used as an index in the `document` array. Because of the different integer sizes, the calculated positions could exceed the array boundaries and cause a segmentation fault.

To fix this, I modified the calculation to simulate 18-bit unsigned integer overflow using modulo `2^18 = 262144`.

```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        // 2 to the power of 18 = 262144
        h = (h * 67 + *s++) % 262144;
    }
    return h;
}
```

The modulo operation keeps the hash within the 18-bit range, reproducing the behavior of the original computer. I did not change the other functions.

I compiled and ran the corrected program using:

```powershell
gcc -Wall -Wextra -g legacy.c -o legacy
./legacy
```

The program executed successfully and produced the following output:

```text
== Diagnostics: ==
 unsigned has 32 bits
 check value: 237614
== End of Diagnostics ==

Attempting to decrypt the document...
== Decrypted Document: ==
cctf{th3_goOD_o1d_D4Ys_w1TH_go0D_0ld_c}
== End of Decrypted Document ==
```

The check value `237614` matches the original developer's diagnostics. The program no longer crashes and successfully decrypts the document.

**Flag:** `cctf{th3_goOD_o1d_D4Ys_w1TH_go0D_0ld_c}`


# Xorcism

**C Language CTF (`cctf`) · Instance `findingcheburashka` · Challenge `xorcism`**

## xorcism

The task was to recover a flag that had been scrambled using XOR encryption.

I examined the `demon.c` source code and found two arrays: `enc`, containing 33 encrypted bytes, and `key`, containing a 4-byte XOR key.

The program checks the entered password using this expression:

```c
enc[i] ^ key[i % 4]
```

The `^` operator performs a bitwise XOR operation, and `i % 4` repeats the key every four characters.

Since XOR is reversible, I could use the same operation to recover the original password.

I added an `encrypt()` function that loops through all 33 encrypted bytes and prints the decrypted characters:

```c
void encrypt()
{
    for (int i = 0; i < PWLEN; i++)
    {
        unsigned char cs = enc[i] ^ key[i % 4];
        printf("%c", cs);
    }
}
```

I also moved the original password verification logic into a separate `enter()` function and called both functions from `main()`:

```c
int main(void)
{
    encrypt();

    printf("\nChecking\n");
    enter();

    return 0;
}
```

This allowed me to display the recovered password first and then verify it using the original password check.

I compiled and ran the modified program:

```powershell
gcc -Wall -Wextra -g demon.c -o demon
.\demon.exe
```

The program produced the following output:

```text
Plea53_d0nT_fE3D_the_NasaL_dEMOnS
Checking
crack me!
password: Plea53_d0nT_fE3D_the_NasaL_dEMOnS
correct! flag: cctf{Plea53_d0nT_fE3D_the_NasaL_dEMOnS}
```

The recovered password passed the original verification, and the program successfully printed the flag.

**Flag:** `cctf{Plea53_d0nT_fE3D_the_NasaL_dEMOnS}`




# Vaslang

**C Language CTF (`cctf`) · Instance `findingcheburashka` · Challenge `vaslang`**

## vaslang

The task was to fix a broken C interpreter for a custom programming language and use it to run `inp.vas`.

I first tried to compile `vaslang.c` on Windows, but the GNU functions `strsep()` and `strchrnul()` were unavailable. I switched to Ubuntu in WSL and compiled it with:

```bash
gcc -D_GNU_SOURCE -Wall -Wextra -g vaslang.c -o vaslang
```

When I ran the original interpreter, it reported many undefined functions, including `upbeat_119` and `main`:

```bash
./vaslang < inp.vas
```

```text
ERROR: func upbeat_119 undefined
...
ERROR: func main undefined
```

I checked where `upbeat_119` was used and defined:

```bash
grep -n -o 'upbeat_119' inp.vas | head -n 5
grep -n '^upbeat_119:' inp.vas | cut -c 1-100
```

The function was referenced on line 127 but defined on line 1716. The problem was that `xdef()` tried to find functions while parsing, before their definitions were available. This is called a *forward reference*.

To fix it, I added a field to `struct term` so that call instructions could store a function name:

```c
struct term {
    char ty;
    union {
        struct term *target;
        unsigned char val;
        char *name;
    };
};
```

Then I changed the function-call branch in `xdef()` to save the name instead of immediately looking up the function:

```c
else {
    ftermp->ty = 'C';
    ftermp->name = term;
}
```

Finally, I changed `case 'C'` in `run()` to find the function when it is actually called:

```c
case 'C': {
    struct term *target = NULL;

    for (int i = 0; funcs + i < funcp; i++) {
        if (strcmp(funcs[i].name, t->name) == 0) {
            target = funcs[i].body;
            break;
        }
    }

    if (target == NULL) {
        errorf("func %s undefined", t->name);
    }

    *fsp++ = t;
    t = target;
    continue;
}
```

The interpreter now searches the function table at runtime. It saves the current instruction before jumping to the called function. The `continue` statement skips the `t++` at the end of the execution loop so that the first instruction of the called function is not skipped.

I rebuilt the program and tested a function calling another function defined later:

```bash
gcc -D_GNU_SOURCE -Wall -Wextra -g vaslang.c -o vaslang
printf 'first: second\nsecond: 65 0\nfirst!\n' | ./vaslang
```

The program produced:

```text
hello
> > > A
>
```

The `A` shows that `first` successfully called `second`, which pushed ASCII value `65` and a zero terminator. The `hello` output comes from the interpreter's built-in test.

This confirms that the forward-reference test works. The full challenge still needs to be verified with:

```bash
./vaslang < inp.vas > output.txt 2>&1
tail -n 5 output.txt
```

**Flag:** Not yet obtained from a verified full run.