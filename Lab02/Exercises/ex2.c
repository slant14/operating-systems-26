#include "stdio.h"

#define BUFFER_SIZE 256

int main() {

    char str[BUFFER_SIZE];
    int ch;
    size_t i = 0;

    while ((ch = getchar()) != '.' && ch != EOF) {
        if (i + 1 < BUFFER_SIZE) 
            str[i++] = (char)ch;
    }
    str[i] = '\0';

    // since you are not reading anything from the input in this program anymore, this step is arbiterary, however make sure to notice, the rest of the input gonna stay in the input buffer until the process of this program exists, and in case of any more reading from input in the same program will result in conflicts!
    if (ch == '.') {
        while ((ch = getchar()) != EOF && ch != '\n') {}
    }


    for (size_t j = i; j > 0; --j) {
        putchar(str[j - 1]);
    }
    putchar('\n');
    return 0;
}
