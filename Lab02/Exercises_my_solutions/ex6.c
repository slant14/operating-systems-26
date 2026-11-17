#include <stdio.h>

int main() {
    int rows = 7;
    int i, j;

    for (i = 1; i <= rows; i++) {
        
        //triangle
        for (j = 1; j <= i; j++) {
            printf("*");
        }

        printf("\t");

        //isosceles triangle
        for (j = 1; j <= (rows - i + 1); j++) {
            printf("*");
        }

        printf("\t");

        //square
        for (j = 1; j <= rows; j++) {
            printf("*");
        }

        printf("\n");
    }

    return 0;
}
