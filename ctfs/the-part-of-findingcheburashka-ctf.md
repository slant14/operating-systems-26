# FindingCheburashka CTF

## legacy

The program needs an 18-bit `unsigned`, but my Mac uses 32 bits. I added an 18-bit mask in `position()`.

```c
unsigned position(const char *s)
{
    unsigned h = 0;

    while (*s) {
        h = (h * 67 + (unsigned char)*s++) & ((1u << 18) - 1);
    }

    return h;
}
```

```bash
clang -std=c17 -Wall -Wextra legacy.c -o legacy
./legacy
```

The check value became `237614`.

**Flag** — `cctf{tHe_g0oD_oLd_DayS_W1TH_9OOd_oLD_C}`

## xorcism

The password is `enc[i] ^ key[i % 4]`. This C program decodes it.

```c
#include <stdio.h>

int main(void)
{
    const unsigned char enc[] = {
        0x47, 0x6d, 0x3f, 0x3b, 0x64, 0x64, 0x05, 0x4b,
        0x07, 0x6f, 0x6d, 0x50, 0x71, 0x64, 0x3f, 0x6b,
        0x68, 0x16, 0x12, 0x4a, 0x68, 0x4f, 0x1b, 0x7c,
        0x56, 0x6d, 0x05, 0x6b, 0x72, 0x4c, 0x6a, 0x61,
        0x44
    };
    const unsigned char key[] = {0x37, 0x21, 0x5a, 0x0f};

    for (size_t i = 0; i < sizeof(enc); i++) {
        putchar(enc[i] ^ key[i % sizeof(key)]);
    }
    putchar('\n');
    return 0;
}
```

```bash
clang -std=c17 -Wall -Wextra solver.c -o solver
./solver
```

Password: `pLe4SE_D0N7_FEed_7HE_nAsaL_dEm0ns`

**Flag** — `cctf{pLe4SE_D0N7_FEed_7HE_nAsaL_dEm0ns}`

## zhAItie

The library asks for data blocks. The first block has 24 bytes. Each call returns the size of the next block. `0` means the end.

```c
typedef struct razzhaitie_ctx razzhaitie_ctx;
typedef void (*razzhaitie_output_cb)(const unsigned char *, int);

static FILE *output_file;

static void write_output(const unsigned char *data, int length)
{
    fwrite(data, 1, (size_t)length, output_file);
}

zhaitie_init();
razzhaitie_ctx *ctx = razzhaitie_new();
size_t requested = 24;

while (requested != 0) {
    buffer = realloc(buffer, requested);
    read_exact(input, buffer, requested);
    requested = (size_t)razzhaitie(
        ctx, buffer, (int)requested, write_output
    );
}
```

```bash
clang -std=c17 -O2 -Wall -Wextra -Werror \
  -o zhaitie-driver zhaitie-driver.c \
  libzhaitie-aarch64-macos.a -lpthread -lm

./zhaitie-driver test.zai presentation.txt
grep -o 'cctf{[^}]*}' presentation.txt
```

**Flag** — `cctf{NatUrAL_INt311iGence_Ov3r_aI_DoC5}`
