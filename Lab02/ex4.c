#include <stdio.h>
#include <ctype.h>

int count(char str[], char character)
{
    int occurrences = 0;
    int i = 0;

    character = (char)tolower((unsigned char)character);

    while (str[i] != '\0')
    {
        if ((char)tolower((unsigned char)str[i]) == character)
        {
            occurrences++;
        }

        i++;
    }

    return occurrences;
}

void countAll(char str[])
{
    int i = 0;
    int first = 1;
    char current;

    while (str[i] != '\0')
    {
        current = (char)tolower((unsigned char)str[i]);

        if (count(str, current) > 0)
        {
            int j = 0;
            int alreadyPrinted = 0;

            while (j < i)
            {
                if ((char)tolower((unsigned char)str[j]) == current)
                {
                    alreadyPrinted = 1;
                    break;
                }

                j++;
            }

            if (!alreadyPrinted)
            {
                if (!first)
                {
                    printf(", ");
                }

                printf("%c:%d", current, count(str, current));
                first = 0;
            }
        }

        i++;
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    countAll(argv[1]);

    return 0;
}
