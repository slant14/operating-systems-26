#include<stdio.h>

int main(void) {
    char s[257];
    char c = getchar();
    int cnt = 0;
    while (c != '.' && c != '\n' && cnt < 256) {
        s[cnt] = c;
        cnt++;
        c = getchar();
    }

    printf("\"");
    for (int i = cnt - 1; i >= 0; i--) {
        putchar(s[i]);
    }
    printf("\"\n");
    return 0;
}