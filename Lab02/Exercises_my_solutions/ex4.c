#include <stdio.h>

int count(char s[], char c) {
    int i = 0;
    int k = 0;
    
    //for case-insensitive comparison
    if (c >= 'A' && c <= 'Z') {
        c = c + 32;
    }

    while (s[i] != '\0') {
        char current = s[i];
        //for case-insensitive comparison
        if (current >= 'A' && current <= 'Z') {
            current = current + 32;
        }
        if (current == c) {
            k++;
        }
        i++;
    }
    return k;
}

void countAll(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        char current = s[i];
        
        ////for case-insensitive comparison
        if (current >= 'A' && current <= 'Z') {
            current = current + 32;
        }

        //check if this character has been already seen
        int isDuplicate = 0;
        int j = 0;
        while (j < i) {
            char past = s[j];
            if (past >= 'A' && past <= 'Z') {
                past = past + 32;
            }
            if (past == current) {
                isDuplicate = 1;
                break;
            }
            j++;
        }

        if (isDuplicate == 0) {
            int c = count(s, current);
            printf("%c:%d", current, c);
            
            //print comma if there are more unique characters ahead
            int k = i + 1;
            int hasMore = 0;
            while (s[k] != '\0') {
                char future = s[k];
                if (future >= 'A' && future <= 'Z') {
                    future = future + 32;
                }
                //check if new symbol is unique
                int seenBefore = 0;
                int m = 0;
                while (m <= i) {
                    char past = s[m];
                    if (past >= 'A' && past <= 'Z') {
                        past = past + 32;
                    }
                    if (past == future) { 
                        seenBefore = 1; 
                        break; 
                    }
                    m++;
                }
                if (!seenBefore) {
                    hasMore = 1;
                    break;
                }
                k++;
            }
            
            if (hasMore) {
                printf(", ");
            }
        }
        i++;
    }
    printf("\n");
}

int main() {
    char s[256];
    scanf("%s", s);
    countAll(s);
    return 0;
}
