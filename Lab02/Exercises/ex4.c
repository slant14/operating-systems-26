#include <stdio.h>
#include <ctype.h>

#define BUFFER_SIZE 256

static size_t count_char(const char *word, int target, size_t len) {
    size_t cnt = 0;
    for (size_t i = 0; i < len; ++i) {
        if (tolower((unsigned char)word[i]) == target) {
            ++cnt;
        }
    }
    return cnt;
}

static void count_all(const char *word, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        int current = tolower((unsigned char)word[i]);
        size_t cnt = count_char(word, current, len);
        printf("%c: %zu%s", current, cnt, (i + 1 < len ? ", " : "\n"));
    }
}

int main(void) {
    char word[BUFFER_SIZE];
    size_t len = 0;
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (len + 1 < BUFFER_SIZE) {         
            word[len++] = (char)ch;
        }
    }
    word[len] = '\0';

    count_all(word, len);
    return 0;
}

