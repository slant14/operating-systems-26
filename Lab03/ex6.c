#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int a[], int l, int h) {
    int pivot = a[h];
    int i = l - 1;
    for (int j = l; j < h; j++) {
        if(a[j] < pivot) {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[h]);
    return i + 1;
}

void QuickSort(int a[], int l, int h) {
    if(l < h) {
        int p = partition(a, l , h);
        QuickSort(a, l, p - 1);
        QuickSort(a, p + 1, h);
    }
}

int main() {
    int a[5] = {2, 7, 10, 3, 5};
    QuickSort(a, 0, 4);
    for (int i = 0; i < 5; i++) {
        printf("%d ", a[i]);
    }
    return 0;
}