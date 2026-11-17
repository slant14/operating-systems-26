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

## Fortforks

The container closes in 5 seconds, so we can write a program to save the result to result.txt file. After a second, we can enter the container and enter the command ```tail -n 5 result.txt```. 

```#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void) {
    pid_t pid = fork();

    if (pid == 0) {
        setsid();

        FILE *result = fopen("result.txt", "w");
        int source = open("secrets", O_RDONLY);

        char buffer[4096];
        ssize_t count;

        while ((count = read(source, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, count, result);
            fflush(result);

            pid = fork();
            if (pid > 0) {
                break;
	        }
        }
    }

    return 0;
}``` \
The obtained flag: threadctf{only_PRopER_LINeAgE_937_T0_LiVe_1n_ca571E}
