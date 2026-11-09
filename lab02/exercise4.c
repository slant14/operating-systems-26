#include <stdio.h>
#include <string.h>
#include <ctype.h>
int count(char string[], char character) {
    int k = 0;
    for (int i=0;i<strlen(string);i++) {
        if (tolower(string[i])==tolower(character)) {
            k+=1;
        }
    }
    return k;
}

int countAll(char string[],char letter) {
    return count(string,letter);
}

int main()
{
    char string[256];
    char character;

    scanf("%s",string);
    scanf(" %c",&character);

    printf("%c:%d\n",tolower(character),count(string,character));

    for (int i=0;i<strlen(string);i++) {
        if (string[i]!=',') {
            printf("%c:%d ",tolower(string[i]),countAll(string,string[i]));
        }
    }
}
