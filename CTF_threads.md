# CTF threads (findingnemo)

## 1. needle
I examined the source code 'Meeting.c'. I noticed that the program uses two threads. One thread retrives the flag from '/root/flag.txt', and the second thread runs the meeting. The program runs the 'meeting()' function. But the problem is that the program does not check what this function does. So, I wrote the 'meeting()' function and change how the meeting works.

I created a 'meeting()' function because this is the function the main program expects to find. First, I open the file containing the flag, then create an array to store the flag. After I read the flag from the file and print it.
```c
#include <stdio.h>
#include <stdlib.h>

void* meeting(void* arg) {
    FILE* f = fopen("/root/flag.txt", "r");
    char flag[256];

    if (f == NULL) {
        printf("Can't find the fabric\n");
        exit(1);
    }

    if (fgets(flag, sizeof(flag), f) != NULL) {
        printf("I got the fabric: %s\n", flag);
    }

    fclose(f);

    exit(0);
    return NULL;
}
```
I compiled and run the program.
```bash
gcc -shared -fPIC -o libmeeting.so main.c -lpthread
./meeting
```
**Answer:** *threadctf{sHOotIng_sOMe_a7oMic_FEeT}*

## 2. princess
I need to destroy 50 cabins with dead princesses without touching the living ones. I checked all processes
```bash
ps aux
```
I see that there are many processes with different names. Also I see that some proncesses has the status Z. This means that they are zombie processes and they had already finished running. So, I need to find these dead princesses and kill their parent processes, which represented the cabins.

I open the directory containig information abour processes, using 'opendir("/proc")'. Then I go through processes one by one, using 'readdir()'. First, I created a path to the process's status file using 'strcat()'. Then I opened this file using 'fopen()' to read information about the process: name, state, and PPid. If the princess was dead, I opened the parent and cheks its name: if name contain 'cabin' I kill the process and print about this.
```c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

int main() {
    int count = 0;

    while (count < 50) {
        DIR *dir = opendir("/proc");
        if (dir == NULL) return 1;

        struct dirent *e;

        while ((e = readdir(dir)) != NULL && count < 50) {
            if (atoi(e->d_name) == 0) continue;

            char path[100] = "/proc/";
            char line[256];
            char cabin[100] = "";

            int dead = 0;
            int princess = 0;
            int parent = 0;

            strcat(path, e->d_name);
            strcat(path, "/status");

            FILE *f = fopen(path, "r");
            if (f == NULL) continue;

            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, "Name:") == line &&
                    strstr(line, "princess_"))
                    princess = 1;

                if (strstr(line, "State:") == line &&
                    strchr(line, 'Z'))
                    dead = 1;

                if (strstr(line, "PPid:") == line)
                    parent = atoi(line + 5);
            }

            fclose(f);

            if (!princess || !dead || parent <= 1)
                continue;

            sprintf(path, "/proc/%d/comm", parent);

            f = fopen(path, "r");
            if (f == NULL) continue;

            fgets(cabin, sizeof(cabin), f);
            fclose(f);

            if (strstr(cabin, "cabin")) {
                if (kill(parent, SIGKILL) == 0) {
                    count++;
                    printf("Destroyed: %d/50\n", count);
                }
            }
        }

        closedir(dir);
        usleep(100000);
    }

    printf("Done!\n");
    return 0;
}
```
I compile and run the program and get:

=== The Forest ===

The forest falls silent. 50 cursed cabins lie in ruin, and

the story is finally over. Your reward waits in your home

directory.
So, I use
```bash
ls
cat flag.txt
```
**Answer:** *threadctf{What_Do_YoU_m3AN_7HEY_4re_4lR3ADY_dEAd}*

## 3. princess2
I need to destroy 100 cabins with dead princesses without touching the living ones. This task is similar to the previous princess, but I couldn't use commands, for example 'ps aux' and so on. 

I open the directory containig information abour processes, using 'opendir("/proc")'. Then I go through processes one by one, using 'readdir()'. First, I created a path to the process's status file using 'strcat()'. Then I opened this file using 'fopen()' to read information about the process: name, state, and PPid. If the princess was dead, I opened the parent and cheks its name: if name contain 'cabin' I kill the process and print about this.
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
        if (dir == NULL) return 1;

        struct dirent *e;

        while ((e = readdir(dir)) != NULL && count < 100) {
            if (atoi(e->d_name) == 0) continue;

            char path[100] = "/proc/";
            char line[256];
            char cabin[100] = "";

            int dead = 0;
            int princess = 0;
            int parent = 0;

            strcat(path, e->d_name);
            strcat(path, "/status");

            FILE *f = fopen(path, "r");
            if (f == NULL) continue;

            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, "Name:") == line &&
                    strstr(line, "princess_"))
                    princess = 1;

                if (strstr(line, "State:") == line &&
                    strchr(line, 'Z'))
                    dead = 1;

                if (strstr(line, "PPid:") == line)
                    parent = atoi(line + 5);
            }

            fclose(f);

            if (!princess || !dead || parent <= 1)
                continue;

            sprintf(path, "/proc/%d/comm", parent);

            f = fopen(path, "r");
            if (f == NULL) continue;

            if (fgets(cabin, sizeof(cabin), f) == NULL) {
                fclose(f);
                continue;
            }
            fclose(f);

            if (strstr(cabin, "cabin")) {
                if (kill(parent, SIGKILL) == 0) {
                    count++;
                    printf("Destroyed: %d/100\n", count);
                    fflush(stdout);
                }
            }
        }

        closedir(dir);
        usleep(100000);
    }

    printf("Done!\n");
    return 0;
}
```
I compiled and run the program and get, then i tried use this command as in the previous task:
```bash
cat flag.txt
```
**Answer:** *threadctf{1_hoPe_y0U_c4N_C_CLEar1y_NoW}*
