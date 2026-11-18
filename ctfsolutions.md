how do we find flags

Dog ctf:
1) BackgroundDog 
we can go to ctf user and right there write ls -a, we will see there is .bash_profile. Then, we can check it by cat .bash_profile and we see that there is a bash command runs when we log in, but it's suspended so we can start it again by fg %1, then we will see outputs like /regular_paw_storage/7d550155b3563a10/Background_dog.png many times, so this is where our flag is.
then we do cp /regular_paw_storage/7d550155b3563a10/Background_dog.png /tmp/

then we simply use scp d.baizigitov-dog@osctf.nnk.moe:/tmp/Background_dog.png (I wont write how I got others out of server in others, so I will only write how I find flags)
then we just send png to the bot
2) CatDog
cd ~/go_to_opt/Pictures
cat Cat_dog_top Cat_dog_bottom > /tmp/cat_dog.png
3) CompileDog
In Programs we can find c code compile_dog.c, running which we see the path to needed flag. it prints /regular_paw_storage/956fee20de7fe951/Compile_dog.png
ғ)  HelloDog
This one is simply hoi:3 in our user ctf ctf/go_to_opt/hoi:3
5) just go to Downloads in ctf/go_to_opt then you simply need head dog.log and tail -n 50 dog.log | grep -oE '[0-9a-f]{8}', this will give you two parts of file where our flag is located 
6) mute dog, again we go to Programs, run ./mute_dog | cat and it will print us needed path


c ctf: 
1) cavern 
cat << 'EOF' > offer.c
#include <stdio.h>
#include <stdlib.h>

extern void* guess(char* your_guess);

void run(void) {
    void* p = guess("we-come-in-peace");
    if (p == NULL) { printf("no offering returned\n"); return; }
    unsigned char* m = (unsigned char*)p;
    printf("HEIST dump: ");
    for (int i = 0; i < 128; i++)
        if (m[i] >= 32 && m[i] < 127)
            putchar(m[i]);
    putchar('\n');
    fflush(stdout);
}
EOF 
then gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
"Wrong," the demons cackle. "Again!"
HEIST dump: $3{cctf{remEmber_tH3re_15_No_sPooN_fre3_FROm_na5A1_deMoNS}
2)
h = (h * 67 + (unsigned char)*s++) & 0x3FFFFu;
just change line with h assignment
gcc -o legacy legacy.c
./legacy
./legacy
== Diagnostics: ==
 unsigned has 32 bits
 check value: 237614
== End of Diagnostics ==

Attempting to decrypt the document...
== Decrypted Document: ==
cctf{7he_gooD_Old_d4yS_W1tH_9ood_0lD_C}
== End of Decrypted Document ==
3) vasals
struct term *fs[512] = {0}; unsigned char vs[512] = {0}; change to struct term *fs[65536] = {0}; unsigned char vs[1048576] = {0};
The interpreter fails because it tries to call functions before they are defined. The writeup says to change how the parser handles calls.
Add char *cname; inside struct term (near target).
errorf("func %s undefined", term); is changed to ftermp->cname = strdup(term);

correct code 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errorf(fmt, ...) {\
    printf("ERROR: " fmt "\n", ##__VA_ARGS__);\
    return;}

struct term {
    char ty;
    union {
        struct term *target;
        unsigned char val;
        char *cname; /* ADDED FOR BUG 1 */
    };
};
struct func {
    char *name;
    struct term *body;
};
gcc -D_GNU_SOURCE -o vaslang vaslang.c
dinislam@dinislam-Star-15:~/Homework4$ ./vaslang
hello
> ^C
dinislam@dinislam-Star-15:~/Homework4$ ./vaslang < inp.vas
bash: inp.vas: No such file or directory
dinislam@dinislam-Star-15:~/Homework4$ ./vaslang < vaslang.vas


struct term terms[1048576] = {0};
struct func funcs[65536] = {0};
struct term *termp = terms;
struct func *funcp = funcs;

/* ADDED FOR BUG 2: Expanded stack sizes */
struct term *fs[65536] = {0}; 
unsigned char vs[1048576] = {0};
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
            /* ADDED FOR BUG 1: Lazy resolution */
            if (t->target == NULL ||
                t->target < terms || t->target >= terms + 1048576) {
                const char* nm = (t->target == NULL) ? NULL : t->cname;
                int found = 0;
                if (nm != NULL) {
                    for (int i = 0; funcs + i < funcp; i++)
                        if (strcmp(funcs[i].name, nm) == 0) {
                            t->target = funcs[i].body;
                            found = 1;
                            break;
                        }
                }
                if (!found) { printf("ERROR: func %s undefined\n", nm ? nm : "?"); return; }
            }
            *fsp++ = t;
            t = t->target-1;
            break;
        case 'R':
            if (fsp == fs) {
                for (vsp = vs; *vsp; putchar(*vsp++));
                putchar('\n');
                return;
            }

            t = *--fsp;
            break;
        }
        t++;
    }
}

void xcmd(char* s)
{
    char *name = strsep(&s, "!");
    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            return run(funcs[i].body);
    errorf("func %s undefined", name);
}

void xdef(const char* sp)
{
    char *s = strdup(sp);
    char *name = strsep(&s, ":");

    struct func *func = funcp;

    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            func = funcs + i;

    struct term *ftermp = termp;
    func->name = name;
    func->body = ftermp;

    while (s != NULL)
    {
        char* term = strsep(&s, " ");
        if (*term == 0)
            continue;

        unsigned char val;
        char* endp;

        if (term[0] == '+' || term[0] == '*' ||
            term[0] == ':' || term[0] == '.') {
            ftermp->ty = term[0];
        } else if (val = strtol(term, &endp,
                10), endp != term) {
            ftermp->ty = 'P';
            ftermp->val = val;
        } else {
            ftermp->ty = 'C';
            ftermp->target = NULL; /* Ensure it starts as NULL */
            for (int i = 0; funcs + i < funcp; i++)
                if (strcmp(funcs[i].name, term) == 0)
                    ftermp->target = funcs[i].body;
            
            /* ADDED FOR BUG 1: Store name if it's undefined yet */
            if (ftermp->target == NULL)
                ftermp->cname = strdup(term); 
        }
        ftermp++;
    }
    ftermp->ty = 'R';
    ftermp++;

    funcp++;
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

thread ctf:
1) ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠄⠹⢅⣀⠹⠒⠊⠀⠀⠀
d.baizigitov-fortforks@osctf.nnk.moe's password: 
You have been deemed worthy
Соединяем вас со специалистом: user = "d.baizigitov", task = "fortforks", container_name = "/sixsevensh-d.baizigitov-fortforks-iwhoqqehax", container_id = "4b91fa4e912ad53007f94f9b972acb2685cb986070e3696ec32484356cb52014"
Connected!
ctf@4b91fa4e912a:~$ Connection to osctf.nnk.moe closed.
dinislam@dinislam-Star-15:~$ 
