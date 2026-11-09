#include <stdio.h>
#include <ctype.h>

int count(char s[], char c) {
    int res = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (tolower(s[i]) == tolower(c)) {
            res++;
        }
    }
    return res;
}

void countAll(char s[]) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%c:%d", tolower(s[i]), count(s, s[i]));
    }
    printf("\n");

}

int main(int argc, char *argv[]) {
    countAll(argv[1]);
    return 0;
}