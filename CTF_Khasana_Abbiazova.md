# Dog CTF

### 1. 01BackgroundDog
We can understand that it is connected with background processes. So I checked:
```bash
ps aux
```
I found there the stopped process: *ctf          238  0.0  0.0    980   656 pts/0    T    15:32   0:00 /usr/local/bin/background_dog /tmp/bgdog*

So I checked
```bash
bg %1
```
and found: */regular_paw_storage/b8baa78c118e5a29/Background_dog.png*

### 2. 01CatDog
I checked the files in directory:
```bash
ls
```
I went to Pictures and checked files in there:
```bash
cd Pictures
ls
```
There I found: *Cat_dog_bottom  Cat_dog_top*

So I combined them and got a complete picture.
```bash
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
```
I took the directory by:
```bash
pwd
```

### 3. 01CompileDog
I went to Programs folder and found *compile_dog.c*

So I compiled and run the program and got: */regular_paw_storage/d3805ef4565a3835/Compile_dog.png*
```bash
gcc compile_dog.c -o compile
./compile
```

### 4. 01EditDog
I went to Doguments folder and fount *EDITME* file. I opened it:
```bash
nano EDITME
```
There was:

Make the dog 4 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                                          .-.
     (___________________________________()6 `-,
     (   _______________________________   /''"`
     //\\                              //\\
jgs  "" ""                             "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

So I did what is asked:

Make the dog 4 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                                              .-.
     (_______________________________________()6 `-,
     (   __________________________________   /''"`
     //\\                                  //\\
jgs  "" ""                                 "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

and got:

*Woof! You have solved Edit_dog!*
*Here's your dog pic: /regular_paw_storage/289d28c64da4fabb/Edit_dog.png*

### 5. 01HelloDog
I looked the files in the starting directory:
```bash
ls
```
and saw *hoi_:3.png*.

### 6. 01HistoryDog

I looked the history:
```bash
history
```
and found */regular_paw_storage/c16ceecd3f1b618a/grandpaw.png*

### 7. 01PoweredDog

The directory is called *go_to_opt*, so it is a hind. 
```bash
cd ..
ls
cd ..
ls
cd ..
ls
```

Here we can find *opt* folder.

```bash
cd opt
ls
cat README.md
```

By doing this we get: */regular_paw_storage/af70531da0c0f6a4/powered_dog.png*

### 8. 01TailheadedDog

I went to Downloads folder and looked what is there:
```bash
cd Downloads
ls
```
I found *dog.log* file. I tried to read it and saw that there was the end of the path, but the file was too big, so I used:
```bash
head -n 20 dog.log
tail -n 20 dog.log
```

I found the beginning and the end of the path:

*/regular_paw_storage/9eb2aaef*

*3baeba30/tailheaded.png*

### 9. 02MuteDog

I went to Programs folder and found *mute_dog*
```bash
cd Programs
ls
```

I tried to run it and there was a picture: *Terminals are listening*

So I tried to write the output to the file:
```bash
./mute_dog > answer.txt
```

I got one part of the path. So I tried to write errors to the output as well.
```bash
./mute_dog > answer.txt 2>&1
cat answer.txt
```

and got: *d933fd0a/Mute_dog.png/regular_paw_storage/e8fdc830* = */regular_paw_storage/e8fdc830d933fd0a/Mute_dog.png*

### 10. 02RemDog

I went to Doguments folder and found there unsorted folder.
```bash
cd Dogumemts
ls
cd unsorted
ls
```

In this folder I found some files and README, where was: *I need to remove all the angry dogs from this folder some day...*
```bash
cat README
```

I looked at files by:
```bash
ls -l
```

and it turned out that angry dogs were files with size 1937, so I deleted them:
```bash
find . -type f -size 1937c -delete
```
and got: *Here's your dog pic: /regular_paw_storage/4fa46cf5a8b89c48/Rem_dog.png*

### 11. 02ScatteredDog

I went to Videos folder and found there 'Scattered dog' folder.
```bash
cd Videos
ls
cd  'Scattered dog'
ls
```

There were files named with numbers from -50 to 49. I tried to combine them into png file and got the flag.
```bash
for i in $(seq -50 49); do cat -- "./$i"; done > ../Scattered_dog.png
```

I got the path by:
```bash
cd ..
pwd
```

### 12. 02ShyDog

I went to Videos folder and found there 'Shy dog' folder.
```bash
cd Videos
ls
cd 'Shy dog'
ls
```

There were a lot of files but I tried to look for png file and there was only one:
```bash
file *
```
*Snapshot_Maybe_Dog_3108badb.png* I moved it to another directory because I couldn't send file from 'Shy dog':
```bash
mv Snapshot_Maybe_Dog_3108badb.png ..
```

### 13. 02SpitoonDog

I went to Programs folder and found spitoon:
```bash
cd Programs
```

I tried to run it:
```bash
./spitoon
```

There was text: 

Welcome to the salty spitoon, how tough are ya?

kc

Listen, kid: come back when you are more of a **man**

I wrote random answer and got a hint that I have to see **man**.
```bash
man spitoon
```

There I saw how I have to answer.
```
Welcome to the salty spitoon, how tough are ya?
How tough am I? How tough am I?! I ran 185 of linux programs this lab
Yea, so?
Without 185 errors^C
Right this way sir: /regular_paw_storage/spitoon/tough_dog.png, sorry to keep you waiting
```

By doing this I got: *regular_paw_storage/b8f5933753560a19/Spitoon_dog.png*

### 14. 03InsecureDog

I went to Doguments folder and found secret_stuff folder:
```bash
cd Doguments
ls
cd secret_stuff
ls
```

There was remember.txt
```bash
cat remember.txt
```
```
I keep forgetting the filename for that sweet secret dog... 
I put it in the super_secret folder inside secret_stuff in my Doguments.

I know the name was a mix of the letters D, O, and G. 
It was four letters long, and one of those letters appeared twice. 
What was it again? DOGG.png? GOGO.png? DOOG.png?
```

I tried to go into super_secret folder but I could not check files in there because I had no permission:
```bash
cd super_secret
ls
```

```
ls: cannot open directory '.': Permission denied
```

So I tried to find the file by:
```bash
for n in {D,O,G}{D,O,G}{D,O,G}{D,O,G}; do     [ -f "$n.png" ] && echo "$n.png"; done
```

and got: *GDGO.png*

### 15. 03NetDog

By the name I could understand that I have to look nets.
```bash
cat /proc/net/tcp
```

```
  sl  local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt   uid  timeout inode                                                     
   0: 00000000:0F90 00000000:0000 0A 00000000:00000000 00:00000000 00000000     0        0 176250286 1 0000000000000000 100 0 0 10 0                 
   1: 00000000:0016 00000000:0000 0A 00000000:00000000 00:00000000 00000000  1000        0 176320615 1 0000000000000000 100 0 0 10 100               
   2: 170011AC:0016 010011AC:AD00 01 00000000:00000000 00:00000000 00000000  1000        0 176320617 3 0000000000000000 20 7 31 10 320 
```

I look for the port with UID 0 and state 0A because UID 0 means that the socket belongs to the root user, while state 0A means that the port is in the LISTEN state and is waiting for incoming connections. So I saved the answer in the file.

```bash
nc -w 3 127.0.0.1 3984 > Net_dog.png
```

### 16. 03RestoreDog

I went to Doguments folder and found crashed.txt
```bash
cd Doguments
ls
cat crashed.txt
```

```
My session crashed while I was writing down where I hid my favorite dog...
Good thing vim keeps swap files around.
I encrypted my notes, so here is the recovery key: 6b425e6754ac2faa
```

So I opened the file in recovery mode and entered there the recovery key:
```bash
vim -r crashed.txt
```
I got: */regular_paw_storage/n23k9i7/Restore_dog.png*

### 17. 03UsedDog

```bash
cd ..
ls
cd ..
ls
cd dog
ls
```

By searching other directories I found *Used_dog.png*

# CCTF

### 1. Cavern

There is a problem in given program.

The guess() function allocates memory for the offering structure, saves the decoded flag in the secret field, and then frees the allocated memory using free(). However, it still returns a pointer to this memory.

This creates a use-after-free vulnerability, because the program returns a pointer to memory that has already been freed.

By looking at the original source code, I understood how the offering structure is organized and found the position of the secret field. I uses this information to try to read the flag directly from memory instead of guessing it.

```c
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define FLAGLEN 55
#define SECRET_OFFSET 64

typedef void* (*guess_function)(char*);

void run(void) {
    dlerror();

    guess_function cavern_guess =
        (guess_function)dlsym(RTLD_DEFAULT, "guess");

    void* ptr = cavern_guess("wrong");

    volatile unsigned char* secret =
        (volatile unsigned char*)ptr + SECRET_OFFSET;

    printf("Flag: ");

    for (int i = 0; i < FLAGLEN; i++) {
        putchar(secret[i]);
    }

    putchar('\n');
}
```

I use dlsym() to find the original guess() function and call it. After examining the source code, I found that this function stores the decoded flag in the secret field, frees the memory, and then returns a pointer to the same memory.

Compile and run:
```bash
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

I got: *cctf{remEMbeR_tH3Re_Is_no_5po0n_FrEE_fRom_N4S41_D3MONS}*

### 2. Legacy

The original program was written for an old architecture where unsigned had 18 bits. On modern computers, unsigned int usually has 32 bits, so the hash function produces different results.

To fix this problem, I added & 0x3FFFF to the hash calculation. This operation keeps only the lowest 18 bits, reproducing the original 18-bit unsigned arithmetic.

As a result, the hash function calculates the correct positions for all encoded strings. The add_data() function can then place each character into the correct position in the document, reconstructing the original flag.

```c
unsigned position(const char *s)
{
    unsigned h = 0;

    while (*s) {
        h = (h * 67 + *s++) & 0x3FFFF;
    }

    return h;
}
```

I got: *cctf{thE_9oOD_0ld_d4y5_Wi7h_go0D_O1D_c}*

### 3. Xorcism

I use XOR decryption to recover the original password.

In the original program, the password is encrypted using the XOR operation. However, the program contains both the encrypted data and the key. This means that I  don't need to guess the password because I can restore it directly.

```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x4d, 0x69, 0x3b, 0x44, 0x44, 0x05, 0x6b, 0x58, 0x4f, 0x2e, 0x50, 0x71, 0x12, 0x1f, 0x6b, 0x68, 0x75, 0x12, 0x6a, 0x68, 0x6f, 0x6e, 0x7c, 0x76, 0x6d, 0x05, 0x4b, 0x72, 0x4c, 0x35, 0x41, 0x64
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    char in[64] = {0};

    fputs("crack me!\n", stdout);
    fputs("password: ", stdout);

    for (int i = 0; i < PWLEN; i++) {
        in[i] = enc[i] ^ key[i % 4];
    }

    in[PWLEN] = '\0';

    printf("correct! flag: cctf{%s}\n", in);

    return 0;
}
```

I got: *cctf{pl34se_dont_F3Ed_THe_N4sAL_DEmoNS}*

# Thread CTF

### 1. Needle

In my solution, I created my own libmeeting.so library with a custom meeting() function. The main program loads this library using dlopen() and finds my function using dlsym(). Inside meeting(), I lock the shared mutex, open /root/flag.txt, read its contents, and return the flag. The main program expects the function to return my name, but it does not check the returned value. Therefore, I return the flag instead of my name. This allows me to get the flag without giving my real name to Polythread.

```bash
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

extern pthread_mutex_t truth_and_order_spell;

void* meeting(void* arg)
{
    (void)arg;

    pthread_mutex_lock(&truth_and_order_spell);

    char* flag = malloc(256);

    FILE* f = fopen("/root/flag.txt", "r");

    if (f == NULL) {
        snprintf(flag, 256, "Could not retrieve fabric");
    } else {
        if (fgets(flag, 256, f) == NULL)
            snprintf(flag, 256, "Could not read fabric");

        fclose(f);
    }

    pthread_mutex_unlock(&truth_and_order_spell);

    return flag;
}
```

I got: *cctf{thE_9oOD_0ld_d4y5_Wi7h_go0D_O1D_c}*

### 2. Princess

First, I started by looking for all running processes and understand how they were organized. I noticed that there were cabin processes and princess processes running at the same time.

```bash
ps aux
```

Then, I used the /proc directory in my C program to inspect the processes automatically. I read information from /proc/[pid]/stat to get the process name, its state, and its parent PID.

My goal was to find princess processes whose names started with princess_ and whose state was Z. This state means that the process had already finished but was still present as a zombie process.

After finding such a princess, I checked its parent process using the PPID. I also used /proc/[pid]/exe to make sure that the parent was actually running the cabin executable.

If all conditions were correct, I sent the SIGTERM signal to the cabin using kill(). After successfully sending the signal, I increased the counter.

I repeated this process until I reached 50 destroyed cabins, because that was the number required by the task.

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
        if (!dir) return 1;

        struct dirent *e;

        while ((e = readdir(dir)) && count < 50) {
            int pid = atoi(e->d_name);
            if (pid <= 1) continue;

            char path[100], line[512], name[256];
            int parent = 0, dead = 0, princess = 0;

            sprintf(path, "/proc/%d/stat", pid);

            FILE *f = fopen(path, "r");
            if (!f) continue;

            if (fgets(line, sizeof(line), f)) {
                char *end = strrchr(line, ')');

                if (end) {
                    *end = '\0';
                    strcpy(name, strrchr(line, '(') + 1);

                    char state;
                    sscanf(end + 2, "%c %d", &state, &parent);

                    if (state == 'Z') dead = 1;
                    if (strncmp(name, "princess_", 9) == 0)
                        princess = 1;
                }
            }

            fclose(f);

            if (!dead || !princess || parent <= 1)
                continue;

            sprintf(path, "/proc/%d/exe", parent);

            char exe[256];
            int len = readlink(path, exe, sizeof(exe) - 1);
            if (len < 0) continue;

            exe[len] = '\0';

            if (strcmp(exe, "/usr/local/bin/cabin") != 0)
                continue;

            if (kill(parent, 0) == 0) {
                if (kill(parent, SIGTERM) == 0) {
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

After compiling and running I got:
```
=== The Forest ===
The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.
```

so I checked the directory and found flag: *threadctf{wha7_Do_Y0U_meaN_tHeY_4R3_AlrEaDY_dEAd}*

```bash
ls
cat flag.txt
```

### 3. Princess2

It is the same as princess, we just can't look at processes bu ps aux and we have to kill 100 processes.

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
        if (!dir) return 1;

        struct dirent *e;

        while ((e = readdir(dir)) && count < 100) {
            int pid = atoi(e->d_name);
            if (pid <= 1) continue;

            char path[100], line[512], name[256];
            int parent = 0, dead = 0, princess = 0;

            sprintf(path, "/proc/%d/stat", pid);

            FILE *f = fopen(path, "r");
            if (!f) continue;

            if (fgets(line, sizeof(line), f)) {
                char *end = strrchr(line, ')');

                if (end) {
                    *end = '\0';
                    strcpy(name, strrchr(line, '(') + 1);

                    char state;
                    sscanf(end + 2, "%c %d", &state, &parent);

                    if (state == 'Z') dead = 1;
                    if (strncmp(name, "princess_", 9) == 0)
                        princess = 1;
                }
            }

            fclose(f);

            if (!dead || !princess || parent <= 1)
                continue;

            sprintf(path, "/proc/%d/exe", parent);

            char exe[256];
            int len = readlink(path, exe, sizeof(exe) - 1);
            if (len < 0) continue;

            exe[len] = '\0';

            if (strcmp(exe, "/usr/local/bin/cabin") != 0)
                continue;

            if (kill(parent, 0) == 0) {
                if (kill(parent, SIGTERM) == 0) {
                    count++;
                    printf("Destroyed: %d/100\n", count);
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

After compiling and running I got:
```
=== The Forest ===
The forest falls silent. 100 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.
```
So I used:
```bash
cat flag.txt
```
 I got: *threadctf{i_h0P3_yoU_Can_C_cl3ArlY_noW}*



