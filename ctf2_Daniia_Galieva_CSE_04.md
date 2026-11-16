## Сavern

in code we can see(address secret buffer is address guess \+ 64\)

```c
#define BUF_SIZE 64
struct offering {
    char guess[BUF_SIZE];
    char secret[BUF_SIZE];
};
```

When we call the guess() function (even with a wrong guess), the program decrypts the flag and puts it into memory. Then it calls free() to "delete" that memory.

The free() command in C doesn't actually erase the data. it just marks the memory as "free" for other things to use. We know the flag is stored exactly 64 bytes from the start of the structure, so we add 64 to the address (+ 64\) and simply read the flag from this "deleted" memory

```c
#include <stdio.h>
#include <string.h>

extern void* guess(char*);

void run(void) {
    void* p = guess("meow") + 64;
    printf("%s", p);
}
```

```
ctf@3438b5117599:~$ nano offer.c
ctf@3438b5117599:~$ gcc -shared -fPIC -o offer.so offer.c -ldl
ctf@3438b5117599:~$ ./cavern ./offer.so
"Wrong," the demons cackle. "Again!"
cctf{RemEM83r_there_is_NO_sPooN_fR3E_FrOM_NA5Al_D3MOn5}
```

## Legacy 

The unsigned variable on the developer's system was 18 bits. In the code, the position() function uses this variable to calculate the string's hash sum. Due to overflow during calculations, the program went outside the document\[69\] array, trying to write data to a huge index. On modern computers, unsigned is 32 or 64 bits, so the hash was calculated incorrectly and the program crashed with a segmentation fault (Segfault).  
We emulated the old 18-bit overflow behavior. We limited the calculation result using the remainder of division by 0x3FFFF (which is 2^18−1, the mask for 18 bits). 

```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h * 67 + *s++) % 0x3FFFF;
    }
    return h;
}
```

```

PS C:\Users\daniyacc\Downloads\Telegram Desktop> gcc legacy.c -o legacy
PS C:\Users\daniyacc\Downloads\Telegram Desktop> ./legacy              
== Diagnostics: ==
 unsigned has 32 bits
 check value: 41006
== End of Diagnostics ==

Attempting to decrypt the document...
== Decrypted Document: ==
cctf{7He_9ooD_o1D_DAYs_with_9o0D_oLD_C}
== End of Decrypted Document ==
```

## Xorcism

The program uses the XOR operation to encrypt the password. Instead of asking the user for input and checking it, we modified the code. We wrote a loop that applies XOR (^) to the encrypted array enc using the key, and saves the result into the in string.

```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x67, 0x10, 0x3f, 0x6e, 0x44, 0x44, 0x05, 0x4b, 0x07, 0x4f, 0x6d, 0x50, 0x71, 0x12, 0x3f, 0x6b, 0x68, 0x55, 0x12, 0x4a, 0x68, 0x6f, 0x1b, 0x3a, 0x56, 0x6d, 0x05, 0x4b, 0x52, 0x6c, 0x6a, 0x41, 0x44
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    char in[64] = {0};
    for (int i = 0; i < PWLEN; i++) {
        in[i] = (enc[i] ^ key[i % 4]);
    }

    printf("correct! flag: cctf{%s}\n", in);
    return 0;
}
```

```
PS C:\Users\daniyacc\Downloads\Telegram Desktop> gcc demon.c -o demon
PS C:\Users\daniyacc\Downloads\Telegram Desktop> ./demon
correct! flag: cctf{P1ease_D0n7_F3ed_tHE_NA5aL_DeM0Ns}
```

