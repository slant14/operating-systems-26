## Cavern

The idea of the solution is that if we are trying to follow the steps of creating secret offer we can give it to function offering check and quarantee that this our answer
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
