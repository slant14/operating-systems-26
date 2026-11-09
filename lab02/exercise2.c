#include <stdio.h>
#include <string.h>
int main()
{
    char s[257];
    scanf("%s",s);

    for (int i=0;i<strlen(s);i++) {
        if (s[i] == '.') {
            s[i]='\0';
            break;
        }
    }
    printf("\"");
    for (int i = strlen(s)-1;i>=0;i--) {
        printf("%c",s[i]);
    }
    printf("\"");
}
