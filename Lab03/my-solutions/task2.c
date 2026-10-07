#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char * const p = malloc(6);
    memset(p, 'A', 6);
    (*p)++;
    p = NULL;
    printf("%c\n", *p);
    free(p);
}