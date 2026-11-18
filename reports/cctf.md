## Cavern

The flag is XOR-decoded into `offering->secret` at runtime, and the offering
is freed before the pointer is returned. `free()` only overwrites the first 16
bytes of the chunk (tcache fd/bk pointers), so `secret` (offset 64) survives.

`run()` is loaded into the same process, so we can call the exported `guess()`
via `dlsym(RTLD_DEFAULT, "guess")`, get back the dangling pointer, and copy the
55-byte secret before the memory is reused.

The struct offering is re-declared locally with the same layout, because struct definitions are not exported symbols and dlsym only gives us the raw pointer.

**In container:**

```bash
cat > offer.c <<'EOF'
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

struct offering { char guess[64]; char secret[64]; };

void run(void) {
    void *(*guess)(char *) = (void *(*)(char *))dlsym(RTLD_DEFAULT, "guess");
    if (!guess) { puts("guess not exported"); return; }
    struct offering *o = (struct offering *)guess((char *)"x");
    char flag[56];
    memcpy(flag, o->secret, 55);
    flag[55] = '\0';
    puts(flag);
}
EOF

gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
# cctf{reMem8ER_ThERe_1S_no_spoON_fREe_frOM_na541_DeMOns}
```

## Legacy

The program was written for a machine where `unsigned` is 18 bits wide (the
diagnostics confirm: `unsigned has 18 bits`, `check value: 237614`). On a
modern 32/64-bit `unsigned`, overflow wraps differently, so `position()`
returns different indices and `document[pos]` writes out of bounds → segfault.

We emulate the original 18-bit unsigned arithmetic by reducing every step
modulo 2^18 — this is exactly what the hardware wraparound did.

**Locally**

```bash
# edit legacy.c, replace the body of position():
nano legacy.c
```
```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h * 67 + *s++) % (1u << 18);
    }
    return h;
}
```

```bash
gcc legacy.c -o legacy
./legacy
# == Diagnostics: ==
#  unsigned has 32 bits
#  check value: 237614
# ...
# cctf{7H3_goOD_o1d_d4y5_witH_g0Od_o1D_c}
```

## Xorcism

The password is not stored in plaintext: `enc[]` holds the password XORed with
the 4-byte repeating `key[]`, and the program compares the user input against
`enc[i] ^ key[i % 4]`. XOR is its own inverse, so decrypting is just applying
the same operation again: `plain[i] = enc[i] ^ key[i % 4]`.

**Locally**

```c
# copy enc[] and key[] from demon.c

#include <stdio.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x10, 0x3f, 0x6e, 0x02, 0x12, 0x05, 0x6b, 0x07, 0x6f, 0x2e, 0x50,
    0x51, 0x44, 0x1f, 0x4b, 0x68, 0x16, 0x32, 0x6a, 0x68, 0x6f, 0x6e, 0x3a,
    0x76, 0x6d, 0x05, 0x4b, 0x04, 0x4c, 0x15, 0x61, 0x44
};
static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void) {
    for (int i = 0; i < PWLEN; i++)
        putchar(enc[i] ^ key[i % 4]);
    putchar('\n');
    return 0;
}
EOF

gcc crack.c -o crack
./crack
# cctf{p1ea53_d0Nt_feED_7he_N45AL_D3mOns}
```