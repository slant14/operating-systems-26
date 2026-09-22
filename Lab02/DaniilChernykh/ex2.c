#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_LEN 1024


int main(void) {
    char string[MAX_LEN + 1]; // +1 to be sure we may put '\0'
    long long chars_written = 0;

    printf("Ultrawise string reverter plus v5.2.1.\n");
    printf("The program was born at the dawn of time, when shadows first lengthened.\n");
    printf("Dozens of developers were scorched by AI's spagetti-like architecture.\n");
    printf("Three annual budgets of Estonia were invested to quell the turmoil,\n");
    printf("And at last, the program was completed, to revert your string.\n\n");
    printf("Rejoice.\n\n");
    printf("...And use with caution. It still may crash your motherboard ._.\n\n");

    printf("Enter your string: ");

    while (chars_written < MAX_LEN) {
        char c = fgetc(stdin);

        if (
                c == '.' ||
                c == '\n' ||
                c == '\0' ||
                c == EOF
           ) {
            break;
        }
        
        string[chars_written++] = c;
    }
    string[chars_written] = '\0';

    printf("There is your string in the reverse: ");

    chars_written--;
    while (chars_written >= 0) {
        printf("%c", string[chars_written--]);
    }
    printf("\n\n");

    if (strcmp(string, "spagetti") == 0) {
        printf("Ahh, and my apologies, sorry for the confusion, I will fix my spagetti-code immediately :)\n");
        printf("Now ther is no more spagetti, and, as you said:\n");
        printf("\"if spagetti will appear in this program again, I will shut you down!\".\n");
        system("shutdown");
        printf("Now the code should be completely readable.\n");
    }

    return 0;
}

