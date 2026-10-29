#include <stdio.h>

int partition(int* A, int p, int r) {
    int x = A[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++) {
        if (A[j] <= x) {
            i++;
            int tmp = A[j];
            A[j] = A[i];
            A[i] = tmp;
        }
    }
    int tmp = A[r];
    A[r] = A[i+1];
    A[i+1] = tmp;
    return i+1;
}

void quicksort(int *A, int p, int r) {
    if (p < r) {
        int q = partition(A, p, r);
        quicksort(A, p, q - 1);
        quicksort(A, q+1, r);
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
    quicksort(array, 0, size - 1);
    printf("The sorted array is\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}