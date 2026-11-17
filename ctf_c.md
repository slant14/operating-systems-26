# C language CTF (cctf)

## cavern

```
#define BUF_SIZE 64
struct offering {
    char guess[BUF_SIZE];
    char secret[BUF_SIZE];
};

void* guess(char* your_guess) {
    struct offering* offering = offering_new();
    offering_set_guess(offering, your_guess);
    offering_set_secret(offering);
    if (offering_check(offering)) {
        printf("The demons nod. \"A lucky guess...\"\n");
    } else {
        printf("\"Wrong,\" the demons cackle. \"Again!\"\n");
    }
    // the demons always set the offering free afterwards
    offering_free(offering);
    return offering;
}
```
As we can see, demons return a pointer to freed offering. Also offering contains `secret` part (after 64 bytes of guess). We can use this pointer with 64 offset to access the secret.
```
#include <stdio.h>
#include <string.h>

extern void* guess(char*);

void run(void) {
    printf("%s", guess("my_lucky_guess") + 64);
}
```

## legacy

```
# original developer's
== Diagnostics: ==
unsigned has 18 bits
check value: 237614
== End of Diagnostics ==

# mine
== Diagnostics: ==
 unsigned has 32 bits
 check value: 3506413614
== End of Diagnostics ==

unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = h * 67 + *s++;
    }
    return h;
}
```
Unsigned h is used to obtain the position in the array where the data should be added (sth like hash function). Unsigned types in C has a specific behavior: when they overflow, they wraps around modulo 2^N, where N is their size in bits. On my machine N = 32, but on developer's machine N = 18, so function `position` works differently. So, to get right positions, we need to take modulo of 262144 = 2^18:

```
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h*67 + *s++) % 262144;
    }
    return h;
}
```

## xorcism

```
int main(void)
{
    char in[64] = {0};


    fputs("crack me!\n", stdout);
    fputs("password: ", stdout);
    if (!fgets(in, sizeof in, stdin))
        return 1;
    in[strcspn(in, "\n")] = '\0';

    # here is a xorcism trick
    for (int i = 0; i < PWLEN; i++) {
        in[i] = (enc[i] ^ key[i % 4]);
    }

    if (strlen(in) != PWLEN) {
        puts("nope");
        return 1;
    }

    for (int i = 0; i < PWLEN; i++) {
        if ((unsigned char)in[i] != (enc[i] ^ key[i % 4])) {
            puts("nope");
            return 1;
        }
    }

    printf("correct! flag: cctf{%s}\n", in);
    return 0;
}
```
This code checks `in[i]` to be equal (`enc[i]` xor `key[i % 4]`). So let's just perform xor(cism) and put it to `in`. 
