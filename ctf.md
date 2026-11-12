# Bash CTF (dogctf)  
The flags are the pictures with dogs.  
The commands similar to (the path differs)
```
scp sofi.sokolova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png .
```
were executed in another terminal.

## 01CompileDog  
```
ls -la  
cd Programs  
ls — la  
gcc compile_dog.c -o dog  
./dog > dog.png
realpath dog.png
cat /home/ctf/go_to_opt/Programs/dog.png
exit
ssh sofi.sokolova-dog@osctf.nnk.moe "cat /regular_paw_storage/ca2fb0dfe01f1f61/Compile_dog.png" > ~/OS/dog.png
```
the output for the cat command is : **/regular_paw_storage/ca2fb0dfe01f1f61/Compile_dog.png**

## 01HelloDog
```
realpath hoi_:3.png
scp sofi.sokolova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png .
```
the full address of the picture is **/home/ctf/go_to_opt/hoi_:3.png**

## 01EditDog
```
ls -la
vim EDITME
scp sofi.sokolova-dog@osctf.nnk.moe:/regular_paw_storage/b1adbd02716f8f6e/Edit_dog.png .
```
after vim EDITME: press **I** (insert), add 7 **_** or spaces (where needed) to make the dog longer, press **Esc**, then type **:x** and press the **Enter**
the output: **Woof! You have solved Edit_dog! Here's your dog pic: /regular_paw_storage/b1adbd02716f8f6e/Edit_dog.png**.  

## 02RemDog
```
ls -la
cd unsorted
ls -la
cat README
cat 07ddfe28
rm 07ddfe28 0ed914a5 0f9c3715 1d833a2a 46f9ffad 4b085187 4c24637a 4f57f485 5c7ad216 68c35455 9734dc0d a9e6014f abac77ec bc6b5a53 e1eb39de eb7e0c55
scp sofi.sokolova-dog@osctf.nnk.moe:/regular_paw_storage/070aa79e93ced837/Rem_dog.png .
```
07ddfe28 was an angry dog,  the file had 1937 chars, i decided to remove all files with the same number of chars.  
The output: **Woof! You have solved Rem_dog! Here's your dog pic: /regular_paw_storage/070aa79e93ced837/Rem_dog.png**

## 01BackgroundDog
```
fg
scp sofi.sokolova-dog@osctf.nnk.moe:/regular_paw_storage/cb5661a2593e6998/Background_dog.png .
```
The output: **/regular_paw_storage/cb5661a2593e6998/Background_dog.png**. Press **ctrl + C**.

## 01HistoryDog
```
history
scp sofi.sokolova-dog@osctf.nnk.moe:/regular_paw_storage/0b176b97cd84ac9c/grandpaw.png .
```
The output: **cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/0b176b97cd84ac9c/grandpaw.png**

## 03RestoreDog
```
ls -la
cat crashed.txt
vim -r .crashed.txt.swp
scp sofi.sokolova-dog@osctf.nnk.moe:/regular_paw_storage/ybclhal/Restore_dog.png .
```
crashed.txt needs to be restored, vim stores swap files.  
**-r** means to recover. After vim command, enter the key (it was provided). The output: **/regular_paw_storage/ybclhal/Restore_dog.png**.

## 03InsecureDog
```
ls -la
cd secret_stuff
ls -la
cat remember.txt
pwd
cat /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/{DOOG,DOGO,DGOO,DGDO,DGOD,DDOG,DODG,DOGD,ODOG,OGOD,OGDO,ODGO,ODGD,ODDG,OGDG,OGGD,OODG,OOGD,GDOO,GODO,GOOD,GDDO,GDOD,GODD,GDOG,GDGO,GODG,GGDO,GGOD,DOGG,DGGO,DGOG,ODGG,OGDD}.png
cat /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/GDOD.png
scp sofi.sokolova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/GDOD.png .
```
Location: **Doguments/secret_stuff**, we search for the fle that actually exists (among all possible variants).

## 01CatDog
```
cd ..
cd ..
cd Pictures
ls -la
cat Cat_dog_top Cat_dog_bottom > cat_dog.png
realpath cat_dog.png
scp sofi.sokolova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Pictures/cat_dog.png .
```
Picture directory contains 2 png files with bottom and top parts of the picture, so we will combine them.    
The path is **/home/ctf/go_to_opt/Pictures/cat_dog.png**.

## 01PoweredDog
```
ls -la
cd ..
cd /opt
ls -la
cat README.md
scp sofi.sokolova-dog@osctf.nnk.moe:/regular_paw_storage/5af2619266839dd3/powered_dog.png .
```
The name of the directory gives a hint to go to the **/opt**. Then read the README.md file.  
The output: **Congrats on discovering your address! Here is your dog image: /regular_paw_storage/5af2619266839dd3/powered_dog.png**

## 03UsedDog
```
cd home
ls -la
cd dog
ls -la
realpath Used_dog.png
scp sofi.sokolova-dog@osctf.nnk.moe:/home/dog/Used_dog.png .
```

## 02MuteDog
```
ls -la
cd Programs
ls -la
./spitoon
./mute_dog
./mute_dog > temp.txt
 cat temp.txt
./mute_dog > temp.txt 2> error.txt
cat temp.txt error.txt
scp sofi.sokolova-dog@osctf.nnk.moe:/regular_paw_storage/76aac6a09a76fb35/Mute_dog.png .
```
The terminals are listening, therefore we need to redirect the stream into files.  
Location: **/regular_paw_storage/76aac6a09a76fb35/Mute_dog.png**


# C language CTF (cctf)

## legacy
The program is old and expects an 18-bit unsigned type, but on my machine unsigned is 32 bits, so the hash must be taken modulo 2^18. An 18-bit unsigned integer holds values from 0 to 2^18 − 1, because 18 binary digits give 2^18 combinations. Applying **hash % 2^18** leaves only the lowest 18 bits, replicating the wraparound behaviour of an 18-bit register on overflow. After fixing the file, position function looks like this:
```
unsigned position(const char *s)
{
	unsigned h = 0;
	while (*s) {
		h = h * 67 + *s++;
	}
	int num = 1; // new
	for (int i = 0; i < 18; i++) { // new
		num *= 2; // new
	} // new
	return h % num; // edited
}
```
Compile the file and get the flag:
```
gcc legacy.c -o legacy && ./legacy
```
The output: 
```
== Diagnostics: ==
 unsigned has 32 bits
 check value: 237614
== End of Diagnostics ==

Attempting to decrypt the document...
== Decrypted Document: ==
cctf{The_GOOD_oLD_dAYS_with_GO0d_olD_c}
== End of Decrypted Document ==
```
The flag is **cctf{The_GOOD_oLD_dAYS_with_GO0d_olD_c}**.  

## xorcism
When we try to run the program, it asks for the password, we will simply print the password.  
The content of the temporary file, the output is the password, then return to the initial file, put the password, and obtain the flag:
```
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
0x47, 0x4d, 0x3f, 0x3b, 0x02, 0x64, 0x05, 0x6b, 0x58, 0x4f, 0x2e, 0x50, 0x71, 0x64, 0x3f, 0x6b, 0x68, 0x55, 0x32, 0x6a, 0x68, 0x4f, 0x1b, 0x7c, 0x56, 0x4d, 0x05, 0x6b, 0x52, 0x4c, 0x6a, 0x41, 0x44
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
  for (int i = 0; i < PWLEN; i++) {
    printf("%c", enc[i] ^ key[i % 4]);
  }
  return 0;
}
```
The output is: **correct! flag: cctf{ple45E_dont_FEed_the_nAsal_dem0Ns}**.  
The flag is **cctf{ple45E_dont_FEed_the_nAsal_dem0Ns}**.  

## vaslang
The errors come from direct references in in inp.vas, the functions call each other before they are defined. The fix is a lazy resolution: a call stores a pointer to the function's slot, and the body is checked at runtime.  
We fixed file:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errorf(fmt, ...) {\
    printf("ERROR: " fmt "\n", ##__VA_ARGS__);\
    return;}

struct func; /* added: forward declaration, so struct term can point to struct func */

struct term {
    char ty;
    union {
        struct func *fn;    /* fixed: was "struct term *target" */
        unsigned char val;
    };
};
struct func {
    char *name;
    struct term *body;  
};

struct term terms[1048576] = {0};
struct func funcs[65536] = {0};
struct term *termp = terms;
struct func *funcp = funcs;

/* added: find a function by name; if absent, create an empty record (body == NULL).
   This allows calling a function before it is defined. */
struct func *getfunc(const char *name)
{
    for (struct func *f = funcs; f < funcp; f++)
        if (strcmp(f->name, name) == 0)
            return f;
    funcp->name = strdup(name);
    return funcp++;
}

struct term *fs[512] = {0};
unsigned char vs[512] = {0};
struct term **fsp = fs;
unsigned char *vsp = vs;

void run(struct term *t)
{
    unsigned l, r;
    fsp = fs;
    vsp = vs;

    while (1) {
        switch (t->ty) {
        case '+':
            l = *--vsp;
            r = *--vsp;
            *vsp++ = (l + r) % 251;
            break;
        case '*':
            l = *--vsp;
            r = *--vsp;
            *vsp++ = (l * r) % 251;
            break;
        case ':':
            l = *--vsp;
            *vsp++ = l;
            *vsp++ = l;
            break;
        case '.':
            vsp--;
            break;
        case 'P':
            *vsp++ = t->val;
            break;
        case 'C':
            /* added: the "undefined" check now happens at run time */
            if (!t->fn->body) { printf("ERROR: func %s undefined\n", t->fn->name); return; }
            *fsp++ = t;
            t = t->fn->body - 1;
            break;
        }
        t++;
    }
}

void xcmd(char* s)
{
    /* fixed: manual search loop replaced by getfunc */
    struct func *f = getfunc(strsep(&s, "!"));
    if (!f->body)
        errorf("func %s undefined", f->name);   /* fixed: check body instead of "not found" */
    run(f->body);
}

void xdef(const char* sp)
{
    char *s = strdup(sp);
    /* fixed: name lookup via getfunc (reuses the record if it was referenced earlier) */
    struct func *func = getfunc(strsep(&s, ":"));

    struct term *ftermp = termp;
    func->body = ftermp;    /* fixed: name is already set by getfunc, only body is assigned */

    while (s != NULL)
    {
        char* term = strsep(&s, " ");
        if (*term == 0)
            continue;

        char* endp;
        long val = strtol(term, &endp, 10);

        if (term[0] == '+' || term[0] == '*' ||
            term[0] == ':' || term[0] == '.') {
            ftermp->ty = term[0];
        } else if (endp != term) {
            ftermp->ty = 'P';
            ftermp->val = val;
        } else {
            ftermp->ty = 'C';
            ftermp->fn = getfunc(term);   /* fixed: was a search for funcs[i].body + "undefined" error; now a lazy reference */
        }
        ftermp++;
    }
    ftermp->ty = 'R';
    ftermp++;

    /* fixed: removed unconditional funcp++ (getfunc creates the slot itself) */
    termp = ftermp;
}

void xline(char *s)
{
    if (strchr(s, '!') != NULL)
        xcmd(s);
    else if (strchr(s, ':') != NULL)
        xdef(s);
    else
        errorf("Failed to parse %s: [command!] or"
               " [definition: body] expected", s)
}

void xrom(const char *s)
{
    char buf[640 * 1024];
    strcpy(buf, s);
    xline(buf);
}

int main()
{
    char buf[640 * 1024];
    xrom("hello: 104 : 248 + : 180 * : 111 0");
    xrom("hello!");
    while (1) {
        printf("> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            return 0;
        *strchrnul(buf, '\n') = 0;
        xline(buf);
    }
}
```

The flag is **cctf{yOU_coulD_hAV3_s0lV3D_tHis_tHe_fuN_waY}**.  

# Threads CTF (threadctf)

## needle
```
ls -la
./meeting
```
The output: **Failed to load library. Cant have a meeting. Error: ./libmeeting.so: cannot open shared object file: No such file or directory**. The photo gives a hint.
We need to write a `program_with_name.c` with a meeting function where we bring the name. The program is sly, so we need to bring the flag as a name, so it will be printed instead of it. The content of the `program_with_name.c`:  
```
#include <stdio.h>
#include <stdlib.h>

void* meeting(void* argument) {
        char* name = calloc(300, 1);
        FILE* file = fopen("/root/flag.txt", "r");
        if (file) {
                fgets(name, 300, file);
                fclose(file);
        }
        return name;
}
```
We will join these two files using the given instruction.
```
Please make sure your `libmeeting.so` is right next to the meeting place.  
You can join it by using `gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread`  
Make sure to bring your name in the meeting function.  
```
```
vim program_with_name.c
gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread
./meeting
```
The output:
```
 Polythread has your concent and name: threadctf{SHOo7INg_SoME_ATOm1c_Fe3t} 
 Good luck, you'll be dyeing the buttons forever now!  
 What did you think was going to happen when you give up your name to fay? 
```
The flag is **threadctf{SHOo7INg_SoME_ATOm1c_Fe3t}**.  

## princess
The princesses are processes, and dead princesses are zombies. We will remove the zombies via killing their parents. We will use `kill <ppid>` of a princess if that princess is in state `<defunct>`.  
(`$1=pid, $2=ppid, $3=state, $4=command`)  
```
ps -eo pid,ppid,state,comm | awk '$3 == "Z" && $4 ~ /princess/ { print $2 }' | xargs -r kill
```
The output: 
```
=== The Forest ===
The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.
```
```
ls -la
cat flag.txt
```
The flag is **threadctf{whAt_D0_Y0u_MeaN_7hEY_aRe_alreADY_DeaD}**.  

## fortforks
The container closes in ~10 seconds, so the commands need to be passed right away.  
`fork()` creates a copy that inherits the open pipe descriptor and gets a fresh lifetime, while the parent exits and the child keeps reading. The flag is at the end of the pipe behind a lot of junk, so I keep only the last chunk read. The first command runs a compiled C program that forks on every read, exits the parent to return to the background, and finally writes the last chunk to `/tmp/out`. The second command simply runs `cat /tmp/out` later, once reading has finished.
```
ssh sofi.sokolova-fortforks@osctf.nnk.moe "echo 'int main(){char b[65536];int n,k=0;if(fork())return 0;while((n=read(0,b,65536))>0){k=n;if(fork())return 0;}write(1,b,k);}' > /tmp/r.c && gcc -w -include unistd.h /tmp/r.c -o /tmp/r && /tmp/r < * > /tmp/out 2>/dev/null"
ssh sofi.sokolova-fortforks@osctf.nnk.moe "cat /tmp/out"
```
The code in the usual view:
```
#include <unistd.h>
int main() {
    char buf[65536];
    int n, k = 0;

    if (fork()) return 0;

    while ((n = read(0, buf, sizeof buf)) > 0) {
        k = n;
        if (fork()) return 0;   // the old one exits, the new one continues to read
    }

    write(1, buf, k);
    return 0;
}
```

The flag is **threadctf{0n1y_pRoP3r_lInEage_GET_To_L1vE_IN_Ca5t1E}**.  
