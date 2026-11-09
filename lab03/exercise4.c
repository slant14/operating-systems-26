#include <stdio.h>

void swap(int*a, int*b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int begin, int end) {

    int pivot = arr[begin];
    int index = begin;

    for (int i = begin + 1; i < end; i++) {
            if (arr[i] < pivot) {
                index++;
                swap(&arr[i],&arr[index]);
            }
        }
    swap(&arr[index],&arr[begin]);
    return index;
    }

    void QuickSort(int arr[],int begin,int end) {
        if (begin < end) {
            int m = partition(arr,begin,end);
            QuickSort(arr,begin,m);
            QuickSort(arr,m+1,end);
        }
    }

int main() {
    int arr[5] = {3,2,5,2,8};
    QuickSort(arr,0,5);
    for (int i = 0; i < 5; i++) {
        printf("%d ",arr[i]);
    }
}
