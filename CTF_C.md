# CTF C (findingcheburashka)

## 1. cavern
I examined the source code of the program 'cavern.c'. I noticed that the 'guess()' function creates a structure that stores a secret. At the end of the function 'free()' function is called, but after that the program stills returns a pointer to the structure. So, I decided to use this pointer to read the secret from memory.

I created a file 'offer.c', made a 'run()' function and defined the same structure as in the source code. This allowed me to access the secret in memory and try to read it.

In 'run()' function I use 'dlsym()' which helps to find 'guess()' function in the main program and call it using 'guess_func()'. Then I called 'guess_func()' with wrong password. Then the flag was written into memory, then the memory was freed, but I had a pointer to it. I used this pointer to read and print the flag.

The 'offer.c' code:
```c
#include <stdio.h>
#include <dlfcn.h>

struct offering {
    char guess[64];
    char secret[64];
};

void run(void) {
    void *(*guess_func)(char *) = dlsym(RTLD_DEFAULT, "guess");

    if (guess_func == NULL) {
        puts("Could not find guess()");
        return;
    }

    struct offering *p = guess_func("test");

    printf("Flag: %.55s\n", p->secret);
}
```

Then, run this code, using
```bash
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```
**Answer:** *cctf{rem3Mber_Th3Re_iS_NO_sPOon_FREe_Fr0M_N45A1_deM0Ns}*

## 2. legacy
I examined the source code 'legacy.c'. I noticed that the program was using the unsigned type to calculate positions in the array. In the diagnostic I see that unsigned type has 18 bits on the original computer, but on my computer it hsa 32 bits. Becausse of this difference the 'position()' function calculated incorreted values, and we get errors.

So, I fix the calculation in the 'position()' function. I used '% 262144' because 2^18 = 262144. This allows to calculate values in the same way as on the old computer with 18-bit unsigned

from this:
```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = h * 67 + *s++;
    }
    return h;
}
```
to this:
```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h * 67 + *s++) % 262144;
    }
    return h;
}
```
Then, I compiled and run the program

**Answer:** *cctf{tHE_GOOd_0ld_daYS_wITH_go0d_0ld_C}*

## 3. xorcism
I examined the source code 'demon.c'. I noticed that the program an arry enc with encrypted data and an array key with 4 values. The program used the XOR operation to check if the entered password was correct. I can use XOR operation to descrypt the passowrd instead of guessing it.

So, I changed the code. The loop goes through all 33 encryped characters. For each character it uses the XOR operation with the corresponding value from the key. The descrypted characters are stored in the array
```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x10, 0x1f, 0x6e, 0x44, 0x12, 0x05, 0x6b, 0x58, 0x6f, 0x2e, 0x50, 0x51, 0x44, 0x69, 0x6b, 0x68, 0x55, 0x12, 0x3c, 0x68, 0x6f, 0x3b, 0x7c, 0x56, 0x10, 0x05, 0x6b, 0x52, 0x4c, 0x6a, 0x41, 0x44
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    char in[64] = {0};

    fputs("crack me!\n", stdout);
    fputs("password: ", stdout);
    if (!fgets(in, sizeof in, stdin))
        return 1;

    for (int i = 0; i < PWLEN; i++) {
        in[i] = (enc[i] ^ key[i % 4]);
    }

    printf("correct! flag: cctf{%s}\n", in);
    return 0;
}
```
I compiled and run the program

**Answer:** *cctf{p1Eas3_doNt_fe3d_tH3_Nasa1_dem0Ns}*
