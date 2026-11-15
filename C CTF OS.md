## cavern
read the code `cavern.c`. 
there are few word about free. It is the idea. `free()` functions unbounds the pointer from the object. But the code doesn't delete the pbject itself. we will use this feature of `free()` function to find the flag.
notice that flag is placed just after the string to check. so the address of this string is 64 symbols later. I just addet 64 to the pointer to string and get the flag

commands:
`gcc -shared -fPIC -o offer.so offer.c`
`./cavern ./offer.so`

code:
```c
#include <stdio.h>

extern void *guess(char *);

void run(void) {
    char my_guess[64] = {0};
    unsigned char *ptr = guess(my_guess);
    printf("%s\n", ptr + 64);
}
```


## legacy
The problem is the size. According to the protocol from the task, size of unsigned on the author's computer is 18. but in my computer it is 32. that is I get incorrect output and even error.

so we will fix this function
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

The only thing we need to do is to change the type of h from `unsigned` to `unsigned _BitInt(18)`:

```c
unsigned _BitInt(18) position(const char *s)
{
    unsigned _BitInt(18) h = 0;
    while (*s) {
        h = h * 67 + (unsigned char)*s++;
    }
    return h;
}
```

and run the code as usuall

## xorcism
We have string and encryption key and we need to encrypt the string. Let's do it.
firstly, I copied data:

```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x4d, 0x1f, 0x6e, 0x64, 0x64, 0x05, 0x6b, 0x58, 0x6f, 0x0e, 0x50, 0x51, 0x44, 0x3f, 0x4b, 0x68, 0x16, 0x12, 0x6a, 0x68, 0x4f, 0x3b, 0x7c, 0x76, 0x10, 0x05, 0x4b, 0x52, 0x6c, 0x35, 0x61, 0x44

};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };
```

And the main function (I led on the source code also)

```c
int main(void)
{
    char password[34];
    for (int i = 0; i < 33; i++) {
        password[i] = enc[i] ^ key[i % 4];
    }
    password[33] = '\0';
    printf("flag:     cctf{%s}\n", password);
    return 0;
}
```
