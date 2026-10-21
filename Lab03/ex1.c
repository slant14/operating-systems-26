#include<stdio.h>
#include<stdlib.h>

int const_tri(int* const p, int n) {
    int temp;
    if(n == 0 || n == 1) {
        return 0;
    }
    if(n == 2) {
        return 1;
    }
    int * const p_x = p;
    p_x[0] = 0;
    p_x[1] = 0;
    p_x[2] = 1;

    for (int i = 3; i <= n; i++) {
        temp = p_x[0] + p_x[1] + p_x[2];
        p_x[0] = p_x[1];
        p_x[1] = p_x[2];
        p_x[2] = temp;
    }

    return p_x[2];
}

int main() {
int const x = 1;
const int *q = &x;
int a, b , c;
int* const p = (int *)malloc(3 * sizeof(int));
p[0] = x;
p[1] = x;
p[2] = 2*x;
printf("%p\n" , &p[0]);
printf("%p\n" , &p[1]);
printf("%p\n" , &p[2]);
if (&p[1] == &p[0] + 1 && &p[2] == &p[1] + 1) {
        printf("The cells are contiguous in memory.\n");
    } 
    else {
        printf("The cells are NOT contiguous in memory.\n");
    }

int n;
scanf ("%d", &n);
int result = const_tri(p, n);
printf("%d\n", result);

free(p);
return 0;
}