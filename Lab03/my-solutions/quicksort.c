#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low-1;

    if (int j=low, j<high-1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
    }

    quickSort(arr, low, p-1);
    quickSort(arr, p+1, high);
}

int main() {
    int array[5] = {5, 4, 3, 2, 1};
    quickSort(array, 0, 4);
    for (int i=0; i<5; i++) {
        printf("%d ", array[i]);
    }
}