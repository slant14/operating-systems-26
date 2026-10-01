#include <stdio.h>
#include <stdlib.h>

int main() {
    int * pc;
    int c;
    c = 22;
    printf("Address of C:%d\n", &c);
    printf("Value of C:%d\n\n", c);

    pc = &c;

    printf("Value of pc:%d\n", pc);
    printf("Value stored in the memory location pointed by pc:%d\n\n", *pc);

    c = 11;

    printf("Value of pc:%d\n", pc);
    printf("Value stored in the memory location pointed by pc:%d\n\n", *pc);

    *pc = 2;

    printf("Address of c:%d\n", &c);
    printf("Value of c:%d\n", c);

    return EXIT_SUCCESS;
}