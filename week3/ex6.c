#include <stdio.h>

int main() {
    int height;
    int width;
    scanf("%d %d", &height, &width);
    printf("\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < i + 1; j++) {
            printf("%c", '*');
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < height/2 + 1; i++) {
        for (int j = 0; j < i + 1; j++) {
            printf("%c", '*');
        }
        printf("\n");
    }
    for (int i = height/2 + 1; i >= 0; i--) {
        for (int j = 0; j < i + 1; j++) {
            printf("%c", '*');
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", '*');
        }
        printf("\n");
    }
    return 0;
}