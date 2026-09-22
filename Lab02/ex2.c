#include <stdio.h>
#include <string.h>
#define MAX_LEN 256

int main() {
    char s[MAX_LEN];
    char c;
    for (int i = 0; i < MAX_LEN - 1; i++) {
        c = getchar();
        if (c == '.' || c == '\n') {
            break;
        }
        s[i] = c;
    }
    for (int j = strlen(s) - 1; j >= 0; j--) {
        printf("%c", s[j]);
    }
    return 0;
}