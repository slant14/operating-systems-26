## Needle

Here is the code for meeting.c
Meeting function as a requirement for meeting is our name, but we need to pass it secretly, e.g. using flag.                     
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
1) After that we should run : gcc -shared -fPIC -o libmeeting.so meeting.c -lpthread
2) Run ./meeting

Obtained flag: threadctf{sHoo71ng_S0mE_atoMic_fe3t}


