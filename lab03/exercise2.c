#include <stdio.h>
#define N 5
void swap(int*a, int*b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void bubble_sort(int arr[]) {
    for (int i = 0; i < N ;i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(&arr[j],&arr[j+1]);
            }
        }
    }
}
int main() {
    int arr[5] = {6,3,7,2,8};
    bubble_sort(arr);
    for (int i = 0; i < N; i++) {
        printf("%d ",arr[i]);
    }
    return 0;
}
