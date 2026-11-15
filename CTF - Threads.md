## Challenge 1 - needle
Our `libmeeting.so` library loads to the same process where `meeting.c` executes and open the file with the flag. Since we execute inside this process we have the same permissions and read and print the flag file inside out meeting function.
```c
#include <stdio.h>  
#include <string.h>  
#include<stdlib.h>  
  
void meeting(){  
    char* flag_fabric_storage = malloc(256);  
    FILE* f = fopen("/root/flag.txt", "r");  
    if (f) {  
        fgets(flag_fabric_storage, 256, f);  
        fclose(f);  
    }  
    printf("%s", flag_fabric_storage);  
    free(flag_fabric_storage);  
}
```

```c
ctf@99492ef3fa99:~$ gcc -shared -fPIC -o libmeeting.so danilanaboishchikov.c -ldl
ctf@99492ef3fa99:~$ ./meeting ./libmeeting.so
threadctf{5h007InG_SoME_4tom1C_Fee7}
Segmentation fault (core dumped)
ctf@99492ef3fa99:~$
```

## Challenge 2 - princess
I used `ps aux` to see all processes. Some princess processes had the state Zombie (dead). 
I used `ps -eo pid,ppid,stat,comm | grep ' Z '` to show Zombi princesses and their parent ID and I killid PPID processes of Zombie princesses. New cabins appeared, so I repeated these steps until I destroyed 50 cabins. Finally, I found `flag.txt` in my home directory and read it using `cat flag.txt`.

## Challenge 3 - princess2
I could not use BASH, so i wrote the following C script to kill all cabins of dead princesses.
```c
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <dirent.h>  
#include <signal.h>  
#include <unistd.h>  
  
int main() {  
    int count = 0;  
  
    while (count < 100) {  
        DIR *dir = opendir("/proc");  
        struct dirent *entry;  
  
        while ((entry = readdir(dir)) != NULL) {  
            int pid = atoi(entry->d_name);  
            if (pid <= 0) continue;  
  
            char path[100], name[64], state;  
            int ppid;  
  
            sprintf(path, "/proc/%d/stat", pid);  
            FILE *f = fopen(path, "r");  
            if (!f) continue;  
  
            int ok = fscanf(f, "%*d (%63[^)]) %c %d",  
                            name, &state, &ppid);  
            fclose(f);  
  
            if (ok != 3) continue;  
  
            if (state == 'Z' && strncmp(name, "princess_", 9) == 0 && ppid > 1) {  
                sprintf(path, "/proc/%d/comm", ppid);  
                f = fopen(path, "r");  
                if (!f) continue;  
  
                fgets(name, sizeof(name), f);  
                fclose(f);  
  
                if (strstr(name, "cabin") && kill(ppid, SIGTERM) == 0)  
                    count++;  
            }  
        }  
  
        closedir(dir);  
        sleep(1);  
        printf("%d\n", count);  
        fflush(stdout);  
    }  
  
    return 0;  
}
```
and 
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

int main() {

    FILE *f = fopen("/home/ctf/flag.txt", "r");
    if (f) {
        char flag[256];
        if (fgets(flag, sizeof(flag), f))
            printf("Flag: %s\n", flag);
        fclose(f);
    }


}

```
to read the flag