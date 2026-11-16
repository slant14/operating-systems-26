## Cavern

The idea behind the solution is that, if we follow the steps for creating a secret offer, we can pass it to the offering_check function and guarantee that this is our answer.

My code of offer.c
```#include <stdio.h>
#include <string.h>
#define BUF_SIZE 64

struct offering {
    char guess[BUF_SIZE];
    char secret[BUF_SIZE];
};


struct offering* offering_new();
void offering_set_secret(struct offering* this);
int offering_check(struct offering* this);
void run(void) {
    
    struct offering* offering = offering_new();
    offering_set_secret(offering);
    for (int i=0;i<BUF_SIZE;i++) {
        offering->guess[i] = offering->secret[i];
    }
    if (offering_check(offering)) {
        printf("%s\n",offering->secret);
    }
}
```
The obtained flag: cctf{reM3mbER_there_1s_nO_sP0oN_FR33_FRoM_Nas4l_deM0n5}

## Legacy
There was a minor problem in the code:
The only thing we needed to change was this line: \
```h = (h * 67 + *s++); ``` \
to this: \
```h = (h * 67 + *s++) & 0x3FFFF;```

Old computers allocated 18 bits for unsigned int, whereas modern computers allocate 32 bits. We simply needed to limit h to 18 bits (3FFFF gives 18 ones)

The obtained flag: cctf{7He_g0Od_old_days_witH_GooD_Old_c}

## xorcism
In this challenge, we can easily delete all unnecessary checks and queries from the code and keep only the function that gives us the password.

```
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x6d, 0x1f, 0x6e, 0x44, 0x12, 0x05, 0x4b, 0x58, 0x4f, 0x2e, 0x50, 0x71, 0x44, 0x3f, 0x4b, 0x68, 0x16, 0x12, 0x6a, 0x68, 0x4f, 0x6e, 0x7c, 0x56, 0x4d, 0x05, 0x6b, 0x52, 0x4c, 0x35, 0x41, 0x02
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    for (int i = 0; i < PWLEN; i++) {
        printf("%c",enc[i] ^ key[i % 4]) {
        }
    }
}

```
The obtained flag: cctf{pLEas3_Dont_FeeD_7He_n4sal_demoN5}
