## Needle

Here is the code for meeting.c.
We can obtain the flag before Polythread asks for our name by opening the file flag.txt and reading its content into the string flag. Then the process is terminated by exit(0).

```
#include <stdio.h>
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
Some processes were zombies. We can use the following command to track zombie processes: ```ps -eo pid, ppid, stat, comm | grep ' Z'
```. After that, these processes can be killed by their PPID using the  ```kill``` command. We need to kill 50 of them.

The obtained flag: threadctf{WHAT_d0_yOu_ME4N_They_arE_a1Re4DY_De4D}

## Fortforks

The container closes in 5 seconds, so we can write a program to save the result to result.txt file. After a second, we can enter the container and enter the command ```tail -n 5 result.txt```. 

```
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void) {
    pid_t pid = fork();

    if (pid == 0) {
        setsid();

        int result = open("result.txt",O_WRONLY | O_CREAT | O_TRUNC,0600);
        int source = open("secrets",O_RDONLY);

        char buffer[4096];
        ssize_t count;

        while ((count = read(source, buffer, sizeof(buffer))) > 0) {
            write(result, buffer, count);

            pid = fork();
            if (pid > 0)
                break;
        }
        close(result);
        close(source);
    }
    return 0;
}
```
The obtained flag: threadctf{only_PRopER_LINeAgE_937_T0_LiVe_1n_ca571E}
