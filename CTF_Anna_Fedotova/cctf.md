# C Language CTF (cctf)

## 1. cavern

`cavern` decodes the real flag into a heap struct, frees it, and `guess()` returns the pointer to that freed memory. `free()` does not erase the bytes, so the flag can be read back. It sits in the struct's second field, at offset 64.

`offer.c`:

```c
#include <stdio.h>
extern void* guess(char* g);
void run(void) {
    char* flag = guess("x");
    printf("%s\n", flag + 64);
}
```

```bash
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

Flag:

```
cctf{remeMbEr_THEr3_1s_nO_SPooN_FRoM_NASA1_D3mon5}
```

## 2. legacy

The program was written for an old machine where `unsigned` was 18 bits, so the hash in `position()` wrapped at 2^18 and landed on a small index into `document[69]`. On a 32-bit machine it does not wrap, the index is huge, and the write goes out of bounds, which is the segfault. Fix: restore the wrap with `% 262144` (= 2^18).

In `position()`, change:

```c
h = h * 67 + *s++;
```

to:

```c
h = (h * 67 + *s++) % 262144;
```

```bash
gcc legacy.c -o legacy
./legacy
```

Flag:

```
cctf{tHE_gooD_olD_d4Ys_w17H_g0oD_oLd_C}
```

## 3. xorcism

The program checks each input character against `enc[i] ^ key[i % 4]`, so that expression is the password itself. Print it, then feed it back to the program.

`solve.c`:

```c
#include <stdio.h>
int main(void) {
    unsigned char enc[33] = {
        0x47,0x6d,0x3f,0x6e,0x02,0x64,0x05,0x4b,0x58,0x6f,0x6d,
        0x50,0x51,0x44,0x3f,0x6b,0x68,0x55,0x32,0x6a,0x68,0x4f,
        0x3b,0x7c,0x76,0x6d,0x05,0x6b,0x52,0x4c,0x35,0x41,0x64
    };
    unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };
    for (int i = 0; i < 33; i++)
        putchar(enc[i] ^ key[i % 4]);
    putchar('\n');
}
```

```bash
gcc solve.c -o solve
./solve                 # password: pLea5E_DoN7_feed_the_nasAL_demoNS
gcc demon.c -o demon
./demon                 # enter the password, it prints the flag
```

Flag:

```
cctf{pLea5E_DoN7_feed_the_nasAL_demoNS}
```
