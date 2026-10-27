#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, 
                void* (*opr)(const void*, const void*)) {
    
    void* result = malloc(size);
    if (!result) return NULL;
    
    if (size == sizeof(int)) {
        *(int*)result = *(int*)initial_value;
    } else if (size == sizeof(double)) {
        *(double*)result = *(double*)initial_value;
    }
    
    for (int i = 0; i < n; i++) {
        void* current_element = (char*)base + i * size;
        void* temp = opr(result, current_element);
        
        if (size == sizeof(int)) {
            *(int*)result = *(int*)temp;
        } else if (size == sizeof(double)) {
            *(double*)result = *(double*)temp;
        }
        
        free(temp);
    }
    
    return result;
}

void* add_int(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(const int*)a + *(const int*)b;
    return result;
}

void* add_double(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(const double*)a + *(const double*)b;
    return result;
}

void* mul_int(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(const int*)a * *(const int*)b;
    return result;
}

void* mul_double(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(const double*)a * *(const double*)b;
    return result;
}

void* max_int(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    int val_a = *(const int*)a;
    int val_b = *(const int*)b;
    *result = (val_a > val_b) ? val_a : val_b;
    return result;
}

void* max_double(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    double val_a = *(const double*)a;
    double val_b = *(const double*)b;
    *result = (val_a > val_b) ? val_a : val_b;
    return result;
}

int main() {
    int int_array[5] = {1, 2, 3, 4, 5};
    double double_array[5] = {1.1, 7.2, 2.8, 6.7, 5.6};
    

    int int_zero = 0;
    int int_one = 1;
    int int_min = INT_MIN;
    double double_zero = 0.0;
    double double_one = 1.0;
    double double_min = -DBL_MAX;
    
    int* int_sum = (int*)aggregate(int_array, sizeof(int), 5, &int_zero, add_int);
    printf("Addition int: %d\n", *int_sum);
    free(int_sum);
    
    int* int_product = (int*)aggregate(int_array, sizeof(int), 5, &int_one, mul_int);
    printf("Multiplication int: %d\n", *int_product);
    free(int_product);
    
    int* int_max = (int*)aggregate(int_array, sizeof(int), 5, &int_min, max_int);
    printf("Int max result: %d\n", *int_max);
    free(int_max);
    
    double* double_sum = (double*)aggregate(double_array, sizeof(double), 5, &double_zero, add_double);
    printf("Addition double: %.2f\n", *double_sum);
    free(double_sum);
    
    double* double_product = (double*)aggregate(double_array, sizeof(double), 5, &double_one, mul_double);
    printf("Multiplication double: %.2f\n", *double_product);
    free(double_product);
    
    double* double_max = (double*)aggregate(double_array, sizeof(double), 5, &double_min, max_double);
    printf("Double max result: %.2f\n", *double_max);
    free(double_max);
    
    return 0;
}