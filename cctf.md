## Cavern

The idea behind the solution is that, if we follow the steps for creating a secret offer, we can pass it to the offering_check function and guarantee that this is our answer.\

My code of offer.c\
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
