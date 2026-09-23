#include "stdio.h"
#include "stdlib.h"

int main() {
    size_t len = 0, buffer_size = 256;
    char* str = malloc(buffer_size);
    if (!str) return -1;

    char ch;
    while((ch = getchar()) != '.' && ch != EOF) {
        if (len + 1 >= buffer_size) {
            size_t extended_size = buffer_size * 2;
            char* temp = malloc(extended_size);
            if (!temp) return -1;

            str = temp;
            buffer_size = extended_size;
        }

        str[len++] = ch;

    }

    str[len] = '\0';
    
    // since you are not reading anything from the input in this program anymore, this step is arbiterary, however make sure to notice, the rest of the input gonna stay in the input buffer until the process of this program exists, and in case of any more reading from input in the same program will result in conflicts!
    if (ch == '.') {
        while ((ch = getchar()) != EOF && ch != '\n') {}
    }

    for (; len > 0; --len) 
        putchar(str[len - 1]);

    putchar('\n');
    fre(str);

    return 0;

}
