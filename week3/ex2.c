#include <stdio.h>

int main(void) {
    char str[256];
    int elem = 0;
    int c;

    while ((c = fgetc(stdin)) != EOF && c != '.' && c != '\n' && c != '\r' && elem < (sizeof(str) - 1)) {
        str[elem] = (char)c;
        elem++;
    }
    printf("\"");
    for (int i = elem - 1; i >= 0; i--) {
        printf("%c", str[i]);
    }
    printf("\"");
    return 0;
}