## Challenge 1 - cavern
Firstly I analysed code and noticed next: code creates a offering struct with user's guess and decrypted right answer. This memory is dynamicelly allcated and never being free. There is a free function, but in never called. Therefore, since my run() function loads to the same process, it has the access to the heap and we can use returning pointer to get the secret information.
```c
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <dlfcn.h>  
  
struct offering {  
    char guess[64];  
    char secret[64];  
};  
  
void run(void) {  
    void* guess = dlsym(RTLD_DEFAULT, "guess");  
    printf("%p", guess);  
    void *(*guess_ptr)(char *) = dlsym(RTLD_DEFAULT, "guess");  
    char* p = guess_ptr("67");  
    while (p != NULL) {  
        printf("%s", p++);  
    }  
}
```
![Screenshot](images/Pasted%20image%2020261110223004.png)
I've got some rubbish in my output, but at the beggining we can find the flag: `cctf{r3MEMbEr_7HEre_i5_No_spoon_fRE3_From_N4S4l_deMOns}`

## Challenge 2 - legacy
Old programmer machine had `sizeof(unsigned) = 18 bits`, but now it is 32 or 64 bits, so since unsigned perform removing of right bits to fit the size, we should perform the same on our machine and save only 18 rightmost bits of the number.
```c
unsigned position(const char *s)  
{  
    unsigned h = 0;  
    while (*s) {  
        h = (h * 67 + *s++) % (unsigned)pow(2, 18);  
    }  
    return h;  
}
```

## Challenge 3 - xorcism
The flag is keeps inside the programm and decodes there, but just do not shown to user, but directly uses on comparions of strings, so we can disable checks and just print decoded flag.
```
#include <stdio.h>  
#include <string.h>  
  
#define PWLEN 33  
  
static const unsigned char enc[PWLEN] = {  
    0x47, 0x10, 0x1f, 0x4e, 0x64, 0x12, 0x05, 0x6b, 0x78, 0x6f, 0x0e, 0x50, 0x71, 0x12, 0x3f, 0x6b, 0x68, 0x75, 0x12, 0x6a, 0x68, 0x4f, 0x1b, 0x5c, 0x56, 0x10, 0x05, 0x6b, 0x52, 0x6c, 0x6a, 0x61, 0x02  
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
  
    printf("cctf{");

	for (int i = 0; i < PWLEN; i++) {
	    char x = enc[i] ^ key[i % 4];
	    printf("%c", x);
	}

printf("}\n");
  
    printf("correct! flag: cctf{%s}\n", in);  
    return 0;  
}  

```
to obtain flag `cctf{p1EAS3_dONT_F3ed_THe_nASa1_deM0n5}`
