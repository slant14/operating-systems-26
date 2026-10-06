#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

void pat1(int n){
    for (int i = 0; i < n; i++){
        for (int j  = 0; j < i + 1; j++){
            printf("*");
        }
        printf("\n");
    }
}

void pat2(int n){
    for (int i = 1; i < n + 1; i++){
        int curr = 0;
        if (i <= n / 2){
            curr = i;
        } else {
            curr = n - i + 1;
        }

        for (int j  = 0; j < curr; j++){
            printf("*");
        }
        printf("\n");
    }
}

void pat3(int n){
    for (int i = 0; i < n; i ++){
        for (int j  = 0; j < n; j++){
            printf("*");
        }
        printf("\n");
    }
}



int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    pat1(n);
    printf("\n");
    pat2(n);
    printf("\n");
    pat3(n);    
}

