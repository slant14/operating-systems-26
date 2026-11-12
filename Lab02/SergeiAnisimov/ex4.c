/* Lab 02 Sergei Anisimov CSE-04 */
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int count(const char text[], int character)
{
    int wanted = tolower((unsigned char)character);
    int occurrences = 0;
    for (size_t i = 0; text[i] != '\0'; ++i)
        occurrences += tolower((unsigned char)text[i]) == wanted;
    return occurrences;
}

void countAll(const char text[])
{
    for (size_t i = 0; text[i] != '\0'; ++i) {
        if (i > 0)
            fputs(", ", stdout);
        printf("%c:%d", tolower((unsigned char)text[i]), count(text, text[i]));
    }
    putchar('\n');
}

int main(int argc, char *argv[])
{
    if (argc != 2 || strlen(argv[1]) > 256) {
        fputs("Supply one string of at most 256 characters.\n", stderr);
        return 1;
    }

    countAll(argv[1]);
    return 0;
}
