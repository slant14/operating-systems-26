## Needle

Here is the code for libmeeting.c
                      
```#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* meeting(void* arg) {
    char flag[256];
    FILE* f = fopen("/root/flag.txt", "r");
    fgets(flag, 256, f);
    exit(0);
    return NULL;
}
```
After that 

