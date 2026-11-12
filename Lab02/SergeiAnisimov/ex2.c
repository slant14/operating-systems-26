/* Lab 02 Sergei Anisimov CSE-04 */
#include <stdio.h>

enum { MAX_LENGTH = 256 };

int main(void)
{
    char text[MAX_LENGTH];
    size_t length = 0;
    int character;

    while ((character = getchar()) != EOF && character != '.' && character != '\n') {
        if (length == MAX_LENGTH) {
            fputs("Input exceeds 256 characters.\n", stderr);
            return 1;
        }
        text[length++] = (char)character;
    }

    putchar('"');
    while (length > 0)
        putchar((unsigned char)text[--length]);
    puts("\"");
    return 0;
}
