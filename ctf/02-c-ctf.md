# C CTF

## [Not Finished] 1. cavern
maybe they are talking about their memory as their are talking about always releasing it.

let's compile our cavern.c
```
gcc cavern.c
./a.out
```
```
Offer up a shared object: ./a.out <your-offering.so>
```
i need to offer something. it has to have run ritual, so it may be a script.

oops read the instruction:
To solve the challenge in the container:

1. Write a C file (e.g. offer.c) that exports void run(void) — the cavern calls it.
2. Build it into a shared object:
   gcc -shared -fPIC -o offer.so offer.c -ldl
3. Run the cavern on your offering:
   ./cavern ./offer.so

## 2. legacy
unsigned has 18 bits
check value: 237614

The program is old and unsigned type has 18 bits. In my machine, unsigned has 32 bits. So, i need to make it fit to 18 bits. We need to make hash mod 2^18. Firstly, fix the file.
```legacy.c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = h * 67 + *s++;
    }
    return h % (262144); // 2^18
}
```
Compile the file:
```
gcc legacy.c
./a.out
```
The output:
```
== Diagnostics: ==
 unsigned has 32 bits
 check value: 237614
== End of Diagnostics ==

Attempting to decrypt the document...
== Decrypted Document: ==
cctf{7H3_goOD_o1d_d4y5_witH_g0Od_o1D_c}
== End of Decrypted Document ==
```
> cctf{7H3_goOD_o1d_d4y5_witH_g0Od_o1D_c}

## [Not Finished] 3. vaslang

let's compile the c file and feed the inp.vas to it
```
gcc vaslang.c
./a.out < inp.vas
```
there were a lot of undefined functions. so, the problem in this language is that the file is using the function is much earlier than defining them. let's fix it in the file:

okay, no. we believe vasilisa more than AI, so let's fix the file from AI :D

## 4. xorcism
```
gcc demon.c
./a.out
crack me!
password:
```
Let's just simply take the password from the code.  
^ - XOR

```demon-break.c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x4d, 0x3f, 0x6e, 0x44, 0x44, 0x05, 0x4b, 0x07, 0x6f, 0x2e, 0x50, 0x71, 0x44, 0x3f, 0x6b, 0x68, 0x75, 0x32, 0x6a, 0x68, 0x6f, 0x3b, 0x5c, 0x03, 0x10, 0x05, 0x6b, 0x52, 0x6c, 0x6a, 0x61, 0x02
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    for (int i = 0; i < PWLEN; i++) {
        printf("%c", enc[i] ^ key[i % 4]);
    }
}
```
> correct! flag: cctf{please_D0Nt_Feed_The_NaS41_deM0n5}

## 5. zhaitie
We are given a library and a zipped file. So, we need to create a file to unzip this file following AGENTS.md. The main idea: read from the file exactly such amount of bytes that the library asks, and write the output, while n is not equal to 0.

```decomp.c
#include <stdio.h>
#include <stdlib.h>

typedef struct razzhaitie_ctx razzhaitie_ctx;
typedef void (*razzhaitie_output_cb)(const void *chunk, int len);

void zhaitie_init(void);
razzhaitie_ctx *razzhaitie_new(void);
int razzhaitie(razzhaitie_ctx *ctx, const void *buf, int len,
               razzhaitie_output_cb cb);

static FILE *out;

static void output(const void *chunk, int len) {
    fwrite(chunk, 1, (size_t)len, out);
}

int main(void) {
    FILE *in = fopen("test.zai", "rb");
    if (in == NULL) return EXIT_FAILURE;

    out = fopen("presentation.out", "wb");
    if (out == NULL) return EXIT_FAILURE;

    zhaitie_init();
    razzhaitie_ctx *ctx = razzhaitie_new();

    unsigned char *buf = NULL;
    size_t cap = 0;

    int n = 24;
    while (n != 0) {
        if ((size_t)n > cap) {
            cap = (size_t)n;
            buf = realloc(buf, cap);
            if (buf == NULL) return EXIT_FAILURE;
        }

        size_t got = fread(buf, 1, n, in);
        if (got != (size_t)n) break;
        
        n = razzhaitie(ctx, buf, n, output);
    }

    free(buf);
    fclose(in);
    fclose(out);
}
```

```
gcc decomp.c libzhaitie-x86_64-linux.a -o decomp
./decomp
cat presentation.out
```
> cctf{NAtuRal_int3lliGenCE_oveR_aI_dOc5}