#include<stdio.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}
int partition(int a[], int l, int r) {
    int p = l;
    int pivot = a[r];
    for (int i = l; i < r; i++) {
        if (a[i] < pivot) {
            swap(&a[i], &a[p]);
            p++;
        }
    }
    swap(&a[p], &a[r]);
    return p;
}
void quicksort(int a[], int l, int r) {
    if (l < r) {
        
        int p = partition(a, l, r);
        quicksort(a, l, p - 1);
        quicksort(a, p + 1, r);
    }
}

int main(void) {
    int arr[] = {9, -3, 5, 2, 6, 8, -6, 1, 3, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
 
    quicksort(arr, 0, n - 1);
    printf("Sorted:  ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}