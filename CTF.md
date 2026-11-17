# DOG CTF
**1. Connect to the server:**

```
ssh v.tumakina-dog@osctf.nnk.moe
```
## 01HelloDog 

**1. Find the image:**

```
ls 
file 'hoi_:3.png'
pwd
```

Found a PNG image in the current directory.

**2. Download the image (Windows PowerShell):**

```
scp 'v.tumakina-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png' dog.png
```

**4. Answer location:**  
`/home/ctf/go_to_opt/hoi_:3.png`

## 01BackgroundDog 

**Commands:**

```
jobs
bg %1
file /regular_paw_storage/5a355e4349fec202/Background_dog.png
```

**Explanation:**

1. `jobs` showed a stopped background process.
    
2. `bg %1` resumed the process.
    
3. The process printed the location of the dog image.
    
4. `file` confirmed that the image was a PNG.
    

**Answer location:**  
`/regular_paw_storage/5a355e4349fec202/Background_dog.png`

## 01HistoryDog 

**Commands:**

```
history
file /regular_paw_storage/363fb4d81b75bdf3/grandpaw.png
```

**Explanation:**

1. Used `history` to display previously executed commands.
    
2. Found command №13, which copied `grandpaw.png` using `cp`.
    
3. Identified the destination path of the image.
    
4. Verified the file format using `file`.
    
5. Downloaded the image using `scp` and submitted it to the Telegram bot.
    

**Answer location:**  
`/regular_paw_storage/363fb4d81b75bdf3/grandpaw.png`

## 01CompileDog 

**Commands:**

```
cd Programs
ls
gcc compile_dog.c -o dog
./dog > result
cat result
file /regular_paw_storage/3c456ff49e660aeb/Compile_dog.png
```

**Explanation:**

1. Navigated to the `Programs` directory.
    
2. Found and inspected `compile_dog.c`.
    
3. Compiled the C source code using `gcc`.
    
4. Executed the compiled program and redirected its output to `result`.
    
5. Read `result` and discovered the image location.
    
6. Verified that the target file was a PNG image.
    

**Answer location:**

`/regular_paw_storage/3c456ff49e660aeb/Compile_dog.png`

## 02MuteDog 

**Commands:**

```
cd /home/ctf/go_to_opt/Programs
```

**Commands:**

```
./mute_dog > output1.txt
cat output1.txt
```

**Explanation:**

Redirected stdout (stream 1) to `output1.txt` and read the first part of the path:

`/regular_paw_storage/f8c7a28e`

**Commands:**

```
./mute_dog 2> output2.txt
cat output2.txt
```

**Explanation:**

Redirected stderr (stream 2) to `output2.txt` to obtain the second part of the path:

`432dbe11/Mute_dog.png`

Combined both parts to get the complete image location.

**Answer location:**

`/regular_paw_storage/f8c7a28e432dbe11/Mute_dog.png`

## 02SpitoonDog 

**Commands:**

```
cd Programs
man spitoon
./spitoon
```

**Explanation:**

1. Found the `spitoon` executable in the `Programs` directory.
    
2. Used `man spitoon` to read the program's instructions.
    
3. Ran the program and entered the required responses:
    

```
How tough am I? How tough am I?! I ran 51 of linux programs this lab
Without 51 errors^C
```

4. The program displayed the location of the dog image.
    

**Command:**

```
file /regular_paw_storage/3256a8606db53031/Spitoon_dog.png
```

**Answer location:**

`/regular_paw_storage/3256a8606db53031/Spitoon_dog.png`

## 01EditDog 

**Commands:**

```
cd Doguments
cat EDITME
nano EDITME
```

**Explanation:**

1. Read `EDITME` and found instructions to make the ASCII dog 9 characters longer.
    
2. Used `nano` to extend the dog's body while preserving its shape and the safety lines.
    
3. Saved the file. The program confirmed successful completion and revealed the image path.
    

**Answer location:**

`/regular_paw_storage/05d2669645f51730/Edit_dog.png`

## 03RestoreDog 

**Commands:**

```
cd Doguments
cat crashed.txt
ls -la
vim -r crashed.txt
```

**Explanation:**

1. Used `cat crashed.txt` to read the hint about a crashed Vim session.
    
2. Used `ls -la` to find the hidden swap file `.crashed.txt.swp`.
    
3. Used `vim -r crashed.txt` to recover the unsaved content from the swap file.
    
4. The recovered text revealed the location of the dog image.
    

**Command:**

```
file /regular_paw_storage/b80htdg/Restore_dog.png
```

**Answer location:**

`/regular_paw_storage/b80htdg/Restore_dog.png`

## 02RemDog 

**Commands:**

```
cd Doguments/unsorted
ls
cat README
cat 0b81163d
cat 24885ecc
ls -lS
rm ....
```

**Explanation:**

1. Opened the `unsorted` directory and found multiple files with random names.
    
2. Read `README`, which instructed us to remove all angry dogs.
    
3. Used `cat` to inspect the ASCII dog images.
    
4. Used `ls -lS` to sort files by size and discovered two groups: 1937 bytes and 1678 bytes.
    
5. Identified the unwanted dog files and deleted them together using `rm` with multiple filenames.
    
6. After removing the unwanted files, the challenge revealed the dog image location.
    

**Answer location:**

`/regular_paw_storage/7a0d2bc9a607145b/Rem_dog.png`

## 03InsecureDog 

**Commands:**

```
cd Doguments/secret_stuff
cat remember.txt
cd super_secret
ls {D,O,G}{D,O,G}{D,O,G}{D,O,G}.png
file DGOD.png
```

**Explanation:**

1. Read `remember.txt` and discovered a hint about the hidden dog image.
    
2. The filename consisted of four letters using only D, O, and G, with one letter repeated.
    
3. Entered the `super_secret` directory, where listing its contents was restricted.
    
4. Used Bash brace expansion to generate possible four-letter filenames.
    
5. Found `DGOD.png` and verified that it was a PNG image.
    

**Answer location:**

`/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/DGOD.png`

## 01TailheadedDog 

**Commands:**

```
cd /home/ctf/go_to_opt/Downloads
head -20 dog.log
tail -20 dog.log
```

**Explanation:**

1. Opened the `Downloads` directory and found `dog.log`.
    
2. Used `head -20` to read the first 20 lines of the file.
    
3. Found the first part of the image path: `/regular_paw_storage/ffa63c7e`.
    
4. Used `tail -20` to read the last 20 lines.
    
5. Found the second part: `abf73a05/tailheaded.png`.
    
6. Combined both parts to obtain the complete path.
    

**Answer location:**

`/regular_paw_storage/ffa63c7eabf73a05/tailheaded.png`

## 01CatDog 

**Commands:**

```
cd Pictures
ls
file Cat_dog_top
file Cat_dog_bottom
cat Cat_dog_top Cat_dog_bottom > cat_dog.png
file cat_dog.png
```

**Explanation:**

1. Found two files: `Cat_dog_top` and `Cat_dog_bottom`.
    
2. Used `file` to identify their formats.
    
3. `Cat_dog_top` contained PNG image data, while `Cat_dog_bottom` contained additional data.
    
4. Used `cat` to concatenate both files into `cat_dog.png`.
    
5. The `>` operator saved the combined output to a new file.
    

**Answer location:**

`/home/ctf/go_to_opt/Pictures/cat_dog.png`

## 02ShyDog 

**Commands:**

```
cd Videos/Shy dog
ls
file *Maybe_Dog*
```

**Explanation:**

1. Opened the `Shy dog` directory and found many files with different names and extensions.
    
2. Used `file *Maybe_Dog*` to check the actual file types of all files containing `Maybe_Dog` in their names.
    
3. Found that `Backup_Maybe_Dog_369b0633.png` was the only file in this group recognized as a PNG image.
    
4. Downloaded the image using `scp` and submitted it to the Telegram bot.
    

**Answer location:**

`/home/ctf/go_to_opt/Videos/Shy dog/Backup_Maybe_Dog_369b0633.png`

## 03UsedDog — 125 points

**Commands:**

```
ls /
cd /home
ls
cd dog
ls
file Used_dog.png
pwd
```

**Explanation:**

1. Used `ls /` to explore the root directory of the Linux filesystem.
    
2. Navigated to `/home` using `cd /home`.
    
3. Used `ls` and discovered the `dog` directory.
    
4. Entered the directory using `cd dog`.
    
5. Found the image `Used_dog.png` using `ls`.
    
6. Used `file Used_dog.png` to verify that it was a PNG image.
    
7. Used `pwd` to identify the current directory.
    

**Answer location:**

`/home/dog/Used_dog.png`

## 01PoweredDog — 125 points

### Commands

```
go to root directory
cd opt
ls
cat README.md
file /regular_paw_storage/9130f20613b84f14/powered_dog.png
```

### Explanation

I was exploring the directories and used `cd ..` several times to return to the root directory `/`.

I noticed the `/opt` directory and decided to investigate it.

Inside `/opt`, I found a file named `README.md`. I opened it using `cat README.md`.

The file displayed a message:

```
Congrats on discovering your address!
Here is your dog image: /regular_paw_storage/9130f20613b84f14/powered_dog.png
```

I used the `file` command to verify that the path contained a PNG image.

### Answer location

```
/regular_paw_storage/9130f20613b84f14/powered_dog.png
```

## 03NetDog 

### Commands

```
cd /usr/local/bin
ls
htop
netdog
cd ~
nc 127.0.0.1 1830 > netdog.png
file netdog.png
```

### Explanation

First, I used `htop` to inspect the running processes on the server.

I received a hint that a process called `netdog` was running and could be accessed through a network port.

I navigated to `/usr/local/bin` and used `ls` to find the `netdog` program.

I ran:

```
netdog
```

The program displayed:

```
Using port: 1830
```

Then I tried to connect using `nc`, but received `Permission denied` because I was in `/usr/local/bin`, where I could not create a file.

I returned to my home directory using `cd ~` and repeated:

```
nc 127.0.0.1 1830 > netdog.png
```

The command appeared to hang, so I stopped it using `Ctrl+C`.

I checked the saved file:

```
ls -l netdog.png
file netdog.png
```

The `file` command confirmed that it was a PNG image measuring 300 × 222 pixels.

### Answer location

```
/home/ctf/netdog.png
```


# C CTF

## cavern

**Commands:**

```text
ls
vim offer.c
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

**Code (`offer.c`):**

```c
#include <stdio.h>
#include <dlfcn.h>

#define FLAGLEN 55

struct offering {
    char guess[64];
    char secret[64];
};

void run(void) {
    void *address = dlsym(RTLD_DEFAULT, "guess");

    void *(*guess_func)(char *) = (void *(*)(char *))address;

    struct offering *ptr = (struct offering *)guess_func("hello");

    printf("Secret: ");
    fwrite(ptr->secret, 1, FLAGLEN, stdout);
}
```

**Explanation:**

1. Read the source code of `cavern` and found that it accepts a shared library (`.so`). It loads the library using `dlopen()` and calls its `run()` function using `dlsym()`.

2. Examined `struct offering`. It contains two arrays: `guess[64]` for the entered guess and `secret[64]` for the real flag.

3. Found that `offering_set_secret()` decodes the flag using XOR and stores 55 decoded bytes in `secret`. The encrypted bytes in the provided source code are only placeholders, so decoding them locally would not reveal the server's flag.

4. Noticed a vulnerability in `guess()`: it calls `offering_free(offering)` and then returns `offering`. This returns a pointer to already freed memory (a dangling pointer).

5. Created `offer.c` with a `run()` function. Since `cavern` loads our shared library into its own process, `run()` can try to find the program's `guess()` function using `dlsym(RTLD_DEFAULT, "guess")`.

6. Converted the returned address into a function pointer and called `guess_func("hello")`. The guess was wrong, but the program still decoded the real flag into `secret` before freeing the structure.

7. Used the returned pointer as `struct offering *ptr` and accessed `ptr->secret`. The memory had been freed, but its contents were still readable during this run. This is a **Use-After-Free (UAF)** read; accessing freed memory is undefined behavior in C and is not guaranteed to work in every environment.

8. Used `fwrite(ptr->secret, 1, FLAGLEN, stdout)` to print exactly 55 bytes without relying on the string being null-terminated.

9. Compiled the file as a shared library with `gcc -shared -fPIC -o offer.so offer.c -ldl` and passed it to `cavern`. The program printed `Wrong`, but the library also printed the real flag.

**Output:**

```text
"Wrong," the demons cackle. "Again!"
Secret: ctf{Rem3mber_ther3_i5_No_sPoOn_FReE_FR0m_nasAl_demon5}
```

**Flag:**

`ctf{Rem3mber_ther3_i5_No_sPoOn_FReE_FR0m_nasAl_demon5}`


## legacy

**Commands:**

```text
gcc legacy.c -o legacy
./legacy
```

**Code change (`legacy.c`):**

Original calculation in `position()`:

```c
h = h * 67 + *s++;
```

Changed to:

```c
h = (h * 67 + *s++) % 262144;
```

**Explanation:**

1. Examined `position()`, which hashes each input string, and `add_data()`, which uses the result as an index in `document[69]`. The document is reconstructed from 33 encoded strings.

2. The original code relied on **18-bit unsigned integers**, but the modern compiler uses **32-bit** `unsigned`. This produced indices outside the array and caused a segmentation fault.

3. Since `2^18 = 262144`, changed the hash update to `(h * 67 + *s++) % 262144` to reproduce the old integer wraparound.

4. After the fix, the encoded strings mapped to valid positions `0`-... .

5. Recompiled and ran `legacy.c`. It reconstructed the document and printed the flag.


**Result:**

```text
cctf{tHe_9oOD_0lD_D4Ys_wiTh_good_o1D_C}
```

## xorcism

**Commands:**

```text
gcc demon.c -o demon
./demon
```

**Code changes (`demon.c`):**

Kept the original `enc` and `key` arrays and added the following code inside `main()` after reading the input:

```c
char answer[64] = {0};

for (int i = 0; i < PWLEN; i++) {
    answer[i] = enc[i] ^ key[i % 4];
}

printf("correct! flag: cctf{%s}\n", answer);
```

**Explanation:**

1. Examined the source code and found that the password must contain exactly 33 characters. Each character is checked against `enc[i] ^ key[i % 4]`.

2. Noticed that the encrypted bytes and the XOR key are both available in the source code, so the correct password can be calculated directly.

3. Modified `main()` to create an array `answer[64]` and decode all 33 characters using XOR. The expression `i % 4` repeats the four-byte key.

4. Printed the decoded password inside `cctf{}` instead of checking the entered password.

5. Recompiled `demon.c` and ran the program. It successfully printed the flag, which was accepted by the CTF bot.

**Output:**

```text
crack me!
password: gv
correct! flag: cctf{PlEA53_donT_fEEd_TH3_n4sal_DEMoNs}
```



# Threads CTF

## needle

**Commands:**

```text
vim steal.c
gcc -shared -fPIC -o libmeeting.so steal.c -pthread -ldl
./meeting
```

**Code (`steal.c`):**

```c
#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

void* meeting(void* arg) {
    pthread_t* retrieval_ptr = (pthread_t*)dlsym(RTLD_DEFAULT, "retrieval");

    void* flag_ptr = NULL;

    int result = pthread_join(*retrieval_ptr, &flag_ptr);

    if (result == 0) {
        printf("Flag: %s\n", (char*)flag_ptr);
        fflush(stdout);
    } else {
        printf("pthread_join failed: %d\n", result);
    }

    return NULL;
}
```

**Explanation:**

1. Examined `meeting.c` and found that the `retrieval` thread reads the flag, while the `meeting` thread runs code from our shared library.

2. Noticed that `retrieval` sleeps for 1 second, but the main thread waits 5 seconds before calling `pthread_join()`. This gives our thread time to get the result first.

3. Created `steal.c` with a `meeting()` function. Used `dlsym()` to find the `retrieval` thread identifier.

4. Called `pthread_join()` before the main thread and received the pointer to the flag in `flag_ptr`.

5. Printed the flag using `printf()` and `fflush()`. The main thread later failed to join the already-joined thread and reported that the fabric was missing.

6. Compiled the shared library and ran `meeting`. The flag was accepted by the CTF bot.

**Output:**

```text
Flag: threadctf{sh0otiNg_SOME_atOM1C_FeEt}

The fabric is nowhere to be found!
Thankfully your name is still with you...
```


## princess

**Commands:**

```bash
ps -eo pid,ppid,stat,cmd | grep 'Z.*princess'
kill 10
ps -eo pid,ppid,stat,cmd | grep 'Z.*princess'
kill 11
kill 13
# Repeated the process check and killed the parents of zombie princesses.
ls
cat flag.txt
```

**Explanation:**

1. Used `ps` and `grep` to find dead princesses. Their process state was `Z` (zombie), while living princesses had state `S`.

2. Found the `PPID` of each zombie princess. This was the ID of its parent process, representing the cursed cabin.

3. Used `kill` on the parent process, not on the zombie princess itself. Checked the process list repeatedly to avoid killing parents of living princesses.

4. Repeated the process until 50 cursed cabins were destroyed. One zombie had `PPID = 1`, but `kill 1` was not permitted, so continued with other processes.

5. After the success message appeared, used `ls` to find `flag.txt` and `cat` to read the flag.

**Output:**

```text
=== The Forest ===
The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.

flag.txt
threadctf{What_d0_You_mEaN_TheY_4Re_41r3adY_DEad}
```



# Threads CTF

## needle

**Commands:**

```text
vim steal.c
gcc -shared -fPIC -o libmeeting.so steal.c -pthread -ldl
./meeting
```

**Code (`steal.c`):**

```c
#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

void* meeting(void* arg) {
    pthread_t* retrieval_ptr = (pthread_t*)dlsym(RTLD_DEFAULT, "retrieval");

    void* flag_ptr = NULL;

    int result = pthread_join(*retrieval_ptr, &flag_ptr);

    if (result == 0) {
        printf("Flag: %s\n", (char*)flag_ptr);
        fflush(stdout);
    } else {
        printf("pthread_join failed: %d\n", result);
    }

    return NULL;
}
```

**Explanation:**

1. Read `meeting.c` and found two threads: `retrieval` reads the flag from `/root/flag.txt`, while `meeting` runs a function from our `libmeeting.so`.

2. Noticed that `retrieval` finishes after `sleep(1)`, but the main thread waits for `sleep(5)` before calling `pthread_join(retrieval, ...)`. This gives our thread time to obtain the result first.

3. Created `steal.c` with a `meeting()` function. Used `dlsym(RTLD_DEFAULT, "retrieval")` to find the address of the `retrieval` thread identifier.

4. Called `pthread_join(*retrieval_ptr, &flag_ptr)` before the main thread. This waited for `retrieval` and stored its returned pointer in `flag_ptr`.

5. Printed the flag using `printf("Flag: %s\n", (char*)flag_ptr)` and `fflush(stdout)`. Then the main thread failed to join the already-joined retrieval thread, so the program reported that the fabric was missing and our name was safe.

6. Compiled `steal.c` as `libmeeting.so` and ran `./meeting`. The flag was accepted by the CTF bot. The solution exploits a race in which our thread joins `retrieval` before the main thread; joining a thread more than once is not valid POSIX usage.

**Output:**

```text
Flag: threadctf{sh0otiNg_SOME_atOM1C_FeEt}

The fabric is nowhere to be found! Polythread is very angry, so you'll have to stay out of faywild for a while.
Thankfully your name is still with you, so you are free to lose it another day.
```

**Flag:**

`threadctf{sh0otiNg_SOME_atOM1C_FeEt}`

## princess

**Commands:**

```text
ssh v.tumakina-princess@osctf.nnk.moe
ps -eo pid,ppid,stat,cmd | grep 'Z.*princess'
kill 10
ps -eo pid,ppid,stat,cmd | grep 'Z.*princess'
kill 11
kill 13
# Repeated the ps command and killed the PPID of each zombie princess.
ls
cat flag.txt
```

**Explanation:**

1. Used `ps` to list processes and their `PID`, `PPID`, state (`STAT`), and command. Filtered the output with `grep 'Z.*princess'` to find dead princesses (`Z`, or zombie processes).

2. Found that each zombie princess had a parent process. The second column (`PPID`) identified its parent, representing the cabin we needed to destroy.

3. Used `kill` on the parent's PPID, not on the zombie princess's own PID. Checked the process list again to avoid touching cabins with living princesses.

4. Repeated this for newly appearing zombie princesses. A zombie whose `PPID` was `1` could not be handled using `kill 1` (`Operation not permitted`), so continued with other parents.

5. After 50 cursed cabins had been destroyed, the challenge displayed a success message. Used `ls` and `cat flag.txt` to read the reward.

**Output:**

```text
=== The Forest ===
The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.

flag.txt
threadctf{What_d0_You_mEaN_TheY_4Re_41r3adY_DEad}
```

**Flag:**

`threadctf{What_d0_You_mEaN_TheY_4Re_41r3adY_DEad}`

## fortforks

**Commands:**

```text
cat > a.c <<'EOF'
# Pasted the C program below, then finished with EOF.
EOF
gcc a.c -o main
./main
# Reconnected after the SSH session was terminated.
tail -c 100 result.txt
```

**Code (`a.c`):**

```c
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

void renew(int s) {
    if (fork() > 0) _exit(0);
    alarm(3);
}

int main() {
    if (fork() > 0) return 0;

    setsid();
    signal(SIGHUP, SIG_IGN);
    signal(SIGALRM, renew);
    alarm(3);

    int in = fileno(fopen("/home/ctf/secrets", "r"));
    int out = fileno(fopen("/home/ctf/result.txt", "w"));

    char b[1024];

    int n = read(in, b, sizeof(b));

    while (n > 0) {
        write(out, b, n);
        n = read(in, b, sizeof(b));
    }
}
```

**Explanation:**

1. The challenge had a named pipe, `/home/ctf/secrets`, with the flag at the end. Reading it took longer than the five-second process limit.

2. Created `a.c` using `cat` . 

3. Used `fork()` to start a child process. The parent exited, while the child used `setsid()` and ignored `SIGHUP` so it could continue after the SSH session disconnected.

4. Set `alarm(3)` and handled `SIGALRM` with `renew()`. Every three seconds the process forked again: the parent exited and the child continued reading using the inherited open file descriptors.

5. Opened `secrets` for reading and `result.txt` for writing. Read the pipe in 1024-byte chunks and saved the data to `result.txt`.

6. Compiled with `gcc a.c -o main` and ran `./main`. After reconnecting to SSH, used `tail -c 100 result.txt` to find the flag at the end of the collected data.

**Output:**

```text
rXlb866 A
Congratulations, Agent. The flag is: threadctf{oNLy_PRoPEr_liNeaGe_9eT_T0_liV3_in_CAsTlE}
```