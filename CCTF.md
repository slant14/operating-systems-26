n.ionov@innopolis.university CSE-04

# CCTF

## cavern

The program loads a shared library and calls its `run()` function. In the
source the structure is two 64-byte arrays, and the flag is decrypted into the
second one:

```c
struct offering {
    char guess[64];
    char secret[64];
};
```

The bug is in `guess()`: it frees the structure and returns the same pointer.
This is a use-after-free. `free()` does not erase the memory, so the flag is
still readable at offset 64.

My library looks up `guess` in the main program and reads 55 bytes after the
first array (`sources/offer.c`):

```c
void run(void)
{
    guess_fn guess = (guess_fn)dlsym(RTLD_DEFAULT, "guess");
    unsigned char *o = (unsigned char *)guess("");
    write(1, o + 64, 55);
}
```

```bash
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

`-fPIC` and `-shared` are needed because the file is loaded with `dlopen`, and
`-ldl` links the `dl*` functions.

Flag: `cctf{rem3M8eR_thErE_Is_NO_Sp0oN_fre3_FrOm_n4sa1_demoNS}`

## legacy

The hash in `position()` is used as an array index. On the original machine
`unsigned` was 18 bits, so the value wrapped modulo 2^18. On a modern machine
the type is wider, the indices are different, and the document falls apart.

I added the mask back:

```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s)
        h = (h * 67u + (unsigned char)*s++) & 0x3ffffu;
    return h;
}
```

```bash
cc -O2 legacy.c -o legacy
./legacy
```

Unsigned overflow is defined in C, so `& 0x3ffff` reproduces the old
arithmetic exactly. The check value `237614` matches the diagnostics from the
task.

Flag: `cctf{tHe_gooD_0LD_days_Wi7H_g0oD_old_C}`

## vaslang

`inp.vas` calls words that are defined later in the file. The interpreter tried
to resolve every name while parsing and stopped with `ERROR: func ...
undefined`.

I made the call deferred: the term keeps the name, and the lookup happens at
run time, when all words are loaded.

```c
struct term {
    char ty;
    union {
        struct term *target;
        unsigned char val;
    };
    char *name;
};
```

```c
/* case 'C': run a call */
if (target == NULL && t->name != NULL) {
    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, t->name) == 0) {
            target = funcs[i].body;
            break;
        }
}
```

```c
/* xdef(): unknown name */
ftermp->ty = 'C';
ftermp->target = NULL;
ftermp->name = strdup(term);
```

```bash
gcc vaslang.c -o vaslang
./vaslang < inp.vas | grep -o 'cctf{[^}]*}'
```

Flag: `cctf{yoU_C0ulD_HAVE_s01v3d_th15_tHE_fUN_w4y}`

## xorcism

The check in `demon.c` compares the input with `enc[i] ^ key[i % 4]`, and the
key is stored next to the ciphertext. So the password is just the XOR of the
two arrays.

```c
for (int i = 0; i < 33; i++)
    putchar(enc[i] ^ key[i % 4]);
```

```bash
cc -O2 xor.c -o xor
./xor
printf '%s' "$(./xor)" | ./demon
```

`printf` without a newline keeps the length at 33, which `demon` checks.

Flag: `cctf{PL3ase_donT_F3ed_7He_NaS4L_dEMOnS}`

## zhaitie

`test.zai` is a compressed stream, and `zhaitie-AGENTS.md` describes the API:
the first call takes 24 bytes, the return value is the size of the next window,
the data comes through a callback, and zero ends the stream.

My driver reads the windows and writes the callback output to a file
(`sources/decompress.c`):

```c
int next = razzhaitie(ctx, buffer, length, emit_output);
if (next == 0) break;
```

```bash
clang -std=c11 -O2 decompress.c libzhaitie-aarch64-macos.a -o decompress
./decompress test.zai presentation.out
tail -n 5 presentation.out
```

On Linux I would use `libzhaitie-x86_64-linux.a` and add `-ldl -lpthread -lm`.
The format itself does not matter here: the library does the work, the driver
only follows the window protocol.

Flag: `cctf{na7uRAL_iNTel1g3Nc3_OvEr_Ai_D0cs}`
