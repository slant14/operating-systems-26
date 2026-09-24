#include <stdio.h>

//inplace bubble sort algorithm
void bubble_sort(int * array, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = size - 1; j >= i + 1; j--) {
            if (array[j] < array[j - 1]) {
                int tmp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = tmp;
            }
        }
    }
}

int main() {
    int size = 0;
    printf("The size of the array is\n");
    scanf("%d", &size);
    printf("Enter the elements one by one:\n");
    int array[size];
    for (int i = 0; i < size; i++) {
        scanf("%d", &array[i]);
    }
    bubble_sort(array, size);
    printf("The sorted array is\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}