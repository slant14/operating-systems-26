## Needle

Here is the code for meeting.c.
The meeting function requires our name as an argument, but we need to pass it secretly, for example using flag. 

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
After that, run:\
 ```gcc -shared -fPIC -o libmeeting.so meeting.c -lpthread ```

And:\
./meeting

The obtained flag: threadctf{sHoo71ng_S0mE_atoMic_fe3t}

## Princess
Some processes were zombies. We can use the following command to see the status of each process (pid, ppid, state): ```ps -eo pid, ppid, state```. After that, these processes can be killed by their PID using the  ```kill``` command.

The obtained flag: threadctf{WHAT_d0_yOu_ME4N_They_arE_a1Re4DY_De4D}


