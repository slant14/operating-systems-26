#include <stdio.h>
#include <string.h>

int count(char *str, char c) {
    int occurrences = 0;
    int len = strlen(str);

    char lower_c = c;
    if (c >= 'A' && c <= 'Z') {
        lower_c = c + 32;
    }
    
    for (int i = 0; i < len; i++) {
        char current = str[i];
        
        if (current >= 'A' && current <= 'Z') {
            current = current + 32;
        }
        
        if (current == lower_c) {
            occurrences++;
        }
    }
    
    return occurrences;
}

void countAll(char *str) {
    int len = strlen(str);
    
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }
        
        int occurrences = count(str, str[i]);
        
        char numStr[16];
        sprintf(numStr, "%d", occurrences);
        
        printf("%c:%s", c, numStr);
        
        if (i < len - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    countAll(argv[1]);
    
    return 0;
}

