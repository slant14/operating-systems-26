#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x10, 0x3f, 0x6e, 0x44, 0x64, 0x05, 0x4b, 0x07, 0x6f, 0x2e, 0x50, 0x51, 0x64, 0x3f, 0x4b, 0x68, 0x55, 0x32, 0x3c, 0x68, 0x4f, 0x1b, 0x3a, 0x56, 0x4d, 0x05, 0x6b, 0x52, 0x6c, 0x6a, 0x41, 0x44
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    char in[64] = {0};
    for (int i = 0; i < PWLEN; i++) {
        printf("%c",(enc[i] ^ key[i % 4]));
    }
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
