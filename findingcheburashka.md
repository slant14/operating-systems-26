- [legacy](legacy)
On this machine, an `unsigned` type was 18 bits wide, so the hash had to be manually truncated to 18 bits after each step. The mask `& 0x3FFFF` is equivalent to taking the remainder modulo 2¹⁸.
```c
#include <limits.h>
#include <stdio.h>


unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h =  (h * 67 + *s++) & 0x3FFFF; // mask `& 0x3FFFF`
    }
    return h;
}

void diagnostics(void)
{
    printf("== Diagnostics: ==\n");
    printf(" unsigned has %d bits\n", (int)(sizeof(unsigned) * CHAR_BIT));
    printf(" check value: %u\n", position("X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"));
    printf("== End of Diagnostics ==\n\n");
}

void add_data(char* document, const char* data)
{
    unsigned pos = position(data);
    document[pos] = *data;
}

void decrypt_document(void)
{
    char document[69] = {0};

    add_data(document, "T0i9T");
    add_data(document, "g1A7T");
    add_data(document, "d0mKc");
    add_data(document, "i0Xhz");
    add_data(document, "d02tH");
    add_data(document, "_0GVh");
    add_data(document, "o0KiL");
    add_data(document, "32Gfh");
    add_data(document, "00BEw");
    add_data(document, "_1yII");
    add_data(document, "H0HaR");
    add_data(document, "c0ehx");
    add_data(document, "L2UeL");
    add_data(document, "y1TRq");
    add_data(document, "o2tuq");
    add_data(document, "O0CDP");
    add_data(document, "_0GVc");
    add_data(document, "d02t9");
    add_data(document, "o0KiO");
    add_data(document, "a1MzO");
    add_data(document, "d02tL");
    add_data(document, "t07CD");
    add_data(document, "_0GVq");
    add_data(document, "G1rpc");
    add_data(document, "10Ikz");
    add_data(document, "h0QBj");
    add_data(document, "_0GVv");
    add_data(document, "d02sx");
    add_data(document, "S0aVR");
    add_data(document, "w0MrP");
    add_data(document, "_0GW8");
    add_data(document, "_0GVl");
    add_data(document, "o0Ki8");

    printf("== Decrypted Document: ==\n");
    printf("cctf{%s}\n", document);
    printf("== End of Decrypted Document ==\n\n");
}

int main()
{
    diagnostics();

    printf("Attempting to decrypt the document...\n");

    decrypt_document();

    return 0;
}
```
- [xorcism](xorcism)
```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x67, 0x6d, 0x3f, 0x6e, 0x44, 0x64, 0x05, 0x6b, 0x58, 0x6f, 0x0e, 0x50, 0x51, 0x64, 0x3f, 0x4b, 0x68, 0x16, 0x32, 0x6a, 0x68, 0x6f, 0x3b, 0x7c, 0x56, 0x6d, 0x05, 0x6b, 0x52, 0x4c, 0x35, 0x41, 0x44
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    char in[64] = {0};

    fputs("crack me!\n", stdout);
    fputs("password: ", stdout);
    if (!fgets(in, sizeof in, stdin))
        return 1;
    in[strcspn(in, "\n")] = '\0';

    if (strlen(in) != PWLEN) {
        puts("nope");
        return 1;
    }

    for (int i = 0; i < PWLEN; i++) {
        if ((unsigned char)in[i] != (enc[i] ^ key[i % 4])) {
            puts("nope");
            return 1;
        }
    }

    printf("correct! flag: cctf{%s}\n", in);
    return 0;
}
```
The `enc` and `key` arrays are copied from `demon.c` unchanged. The program performs the same operation as the check in the original code `enc[i] ^ key[i % 4]` but prints the result instead of performing a comparison.
```c
#include <stdio.h>

int main(void)
{
    unsigned char enc[33] = {
        0x67, 0x6d, 0x3f, 0x6e, 0x44, 0x64, 0x05, 0x6b, 0x58, 0x6f, 0x0e,
        0x50, 0x51, 0x64, 0x3f, 0x4b, 0x68, 0x16, 0x32, 0x6a, 0x68, 0x6f,
        0x3b, 0x7c, 0x56, 0x6d, 0x05, 0x6b, 0x52, 0x4c, 0x35, 0x41, 0x44
    };
    unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

    for (int i = 0; i < 33; i++)
        putchar(enc[i] ^ key[i % 4]);
    putchar('\n');
    return 0;
}
```
- [vaslang](vaslang)
Previously, a call instruction stored the memory address of the target function’s code—an address that could only be determined once the function had already been read. Now, the target stores the function’s name, which is known immediately, even if the function is defined further down in the file.
```c
struct term {
    char ty;
    union {
        char *target;
        unsigned char val;
    };
};
```
Previously, there was a step here that looked up a function in a table; if it hadn't been defined yet, an "undefined" error would be thrown. Now, that lookup is gone: the name is simply stored within the command. We will perform the function lookup later, once the entire file has been read.
```c
if (term[0] == '+' || term[0] == '*' ||
            term[0] == ':' || term[0] == '.') {
            ftermp->ty = term[0];
        } else if (val = strtol(term, &endp,
                10), endp != term) {
            ftermp->ty = 'P';
            ftermp->val = val;
        } else {
            ftermp->ty = 'C';
            ftermp->target = term;
        }
        ftermp++;
```
When a call is made, the loop iterates through all the functions and locates the required one using `strcmp`. Then, `t` is set to `f->body - 1` the subtraction of one is necessary because there is a `t++` at the end of the `while` loop, ensuring that execution begins precisely at the first instruction of the function. A `break` statement is mandatory; otherwise, since `t` would point to a different instruction after the assignment, the loop would continue by reading garbage data.
```c
case 'C':
            *fsp++ = t;
            for (struct func *f = funcs; f < funcp; f++) {
                if (strcmp(f->name, t->target) == 0) {
                    t = f->body - 1;
                    break;
                }
            }
            break;
```
