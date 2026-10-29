#include <stdio.h>
#include <ctype.h>
#include <string.h>

int count(char str[], char c) {
    int cnt = 0;
    c = tolower(c);
    for (int i = 0; i < strlen(str); i++) {
        if (tolower(str[i]) == c) {
            cnt++;
        }
    }
    return cnt;
}

void countAll(char str[]) {
    for (int i = 0; i < strlen(str); i++) {
        printf("%c:%d", str[i], count(str, str[i]));
        if (i != strlen(str) - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    char str[256];
    strcpy(str, argv[1]);
    countAll(str);
    return 0;
}