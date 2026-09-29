#include <stdio.h>
#include <string.h>

int main(void)
{
    char str[257];
    int length = 0;
    int ch;

    while (length < 256)
    {
        ch = getchar();

        if (ch == '.' || ch == '\n' || ch == EOF)
            break;

        str[length] = (char)ch;
        length++;
    }

    str[length] = '\0';

    printf("\"");

    for (int i = (int)strlen(str) - 1; i >= 0; i--)
    {
        putchar(str[i]);
    }

    printf("\"\n");

    return 0;
}
