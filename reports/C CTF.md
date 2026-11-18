## Challenge: cavern

The original program `cavern` loads a shared library (`.so`) using `dlopen()` and executes its `run()` function using `dlsym()`.

The program defines a structure containing two arrays:

```
#define BUF_SIZE 64

struct offering {
    char guess[BUF_SIZE];
    char secret[BUF_SIZE];
};
```

`guess` stores our guess, while `secret` stores the real flag. Each array occupies 64 bytes, so `secret` starts at offset 64. The flag is decrypted using XOR in `offering_set_secret()`:

```
this->secret[i] = fenc[i] ^ fkey[i % 4];
```

The vulnerability is in the original `guess()` function. It calls `offering_free(offering)` to release the allocated memory and then returns the same pointer. This is a **Use-After-Free (UAF)** vulnerability. The memory may still contain the decrypted flag even after being freed.

To exploit this vulnerability, I create my own shared library that calls the original `guess()` function with an empty string. The function returns a pointer to the freed memory, allowing me to read the flag directly from the `secret` field.

My source code `offering.c`:

```
#include <stdio.h>
#include <string.h>

extern void *guess(char *);

void run(void)
{
    unsigned char *p = guess("");
    char flag[56];

    memcpy(flag, p + 64, 55);
    flag[55] = '\0';

    printf("%s\n", flag);
}
```

The `extern` declaration allows our library to call the original `guess()` function. The `run()` function is the entry point that `cavern` executes. We call `guess("")` and save the returned pointer in `p`. Then `memcpy()` copies 55 bytes from offset 64, where the `secret` field is located, into our local array. We add `'\0'` to terminate the string and use `printf()` to display the flag.

I create and edit the source file using Vim:

```
vim offering.c
```

Next, I compile the shared library:

```
gcc -fPIC -shared offering.c -o offering.so
```

Finally, I run the challenge with our library:

```
./cavern ./offering.so
```

The program loads our library, finds `run()` and executes our code. It calls the vulnerable function, reads the flag from the freed memory and prints it.
## Challenge: legacy

The problem is in the `position()` function in `legacy.c`. It calculates a hash by multiplying the current value by 67 and adding each input character. Diagnostics showed that the original system used an 18-bit `unsigned` type, but the current calculation did not preserve this 18-bit limit.

To fix this, I added `% 262144` inside the `while` loop. Since 262144 equals (2^{18}), it keeps the hash within the original 18-bit range after every iteration.

I opened and edited `legacy.c` using Vim. The modified line is:

```
h = (h * 67 + *s++) % 262144;
```

After saving the changes, I compiled and executed the program. It successfully decrypted the document and printed the flag directly to the terminal.

## Challenge: xorcism

The original `main()` function in `demon.c` checks the entered password against data encrypted using a repeating XOR key. The important part is the expression `enc[i] ^ key[i % 4]`, which applies the same 4-byte key repeatedly.

Since XOR is reversible, I can recover the original password by applying the same operation again. I replaced the original `main()` function with a simpler version that decodes the encrypted data and prints the result directly.

I edited `demon.c` using Vim. The new function is:

```
int main(void)
{
    printf("cctf{");

    for (int i = 0; i < PWLEN; i++)
        putchar(enc[i] ^ key[i % 4]);

    printf("}\n");
    return 0;
}
```

The loop decodes each character using the repeating XOR key. `putchar()` prints the decoded characters one by one, while `printf()` adds the flag prefix and closing brace.