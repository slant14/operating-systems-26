#include <stdio.h>
#include <string.h>

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

int count (char s[], char c) {
    int length = 0;
    for (int i = 0; i < strlen(s); i++) {
        if (to_lower(s[i]) == to_lower(c)) {
            length++;
        }
    }
    return length;
}

void countAll (char s[]) {
    int first = 1;
    for (int i = 0; i < strlen(s); i++) {
        int repeat = 0;
        for (int j = 0; j < i; j++) {
        if (to_lower(s[j]) == to_lower(s[i])) {
            repeat = 1;
            break;
        }
           if (repeat) {
            continue;
        }
    }
        int n = count(s, s[i]);
        if (!first) {
            printf(", ");
        }
        printf("%c:%d", to_lower(s[i]), n);
        first = 0;  
}
    printf("\n");
}

int main(void) {
    char line[256];
    char s[256];
    char c;
    scanf("%255[^,], %c", s, &c);
    printf("%c:%d\n", c, count(s, c));
    countAll(s);

    return 0;
}