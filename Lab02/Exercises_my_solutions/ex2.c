#include <stdio.h>
#include <string.h>
#define MAX 256
int main(){
    char str[MAX];
    int c, i = 0;
    
    while (i < MAX - 1) {
        c = getchar();
        if (c == '.' || c == '\n' || c == EOF)
            break;
        str[i++] = (char)c;
    }
    str[i] = '\0';

    printf("\"");
    for (int j = i - 1; j >= 0; j--)
        printf("%c",str[j]);
    printf("\"\n");

    return 0;
}
