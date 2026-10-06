#include <stdio.h>
#include <string.h>

int tribonacci(int n){
    int b1 = 0;
    int b2 = 1;
    int b3 = 1;

    if (n == 0){
        return 0;
    } else if (n == 1 || n == 2){
        return 1;
    } else {
        for (int i = 0; i < n - 2; i++){
            int temp = b1 + b2 + b3;
            b1 = b2;
            b2 = b3;
            b3 = temp;
        }
    }

    return b3;
}

int main() {
    printf("%d\n", tribonacci(4));
    printf("%d\n", tribonacci(36));
    
    return 0;
}

