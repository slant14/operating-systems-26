#include <stdio.h>
#include <string.h>

#define MAX_SIZE 256

int main() {
    char str[MAX_SIZE];
    int i = 0;
    char ch;

    while (i < MAX_SIZE - 1) {
        ch = getchar();
       
        if (ch == '.' || ch == '\n') {
            break;
        }
        
        str[i] = ch;
        i++;
    }
    
    str[i] = '\0';
    
    for (int j = i - 1; j >= 0; j--) {
        printf("%c", str[j]);
    }
    printf("\n");
    
    return 0;
}
