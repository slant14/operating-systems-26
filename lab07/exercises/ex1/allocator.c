#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 10000000

unsigned int arr[N] = {0};

void allocate_best_fit(unsigned int adrs, int size);
void allocate_first_fit(unsigned int adrs, int size);
void allocate_worst_fit(unsigned int adrs, int size);
void clear(unsigned int adrs);
void run_queries(void (*allocate)(unsigned int, int), const char* name);

int main() {
    run_queries(allocate_first_fit, "first fit");
    printf("\n");
    run_queries(allocate_best_fit,  "best fit");
    printf("\n");
    run_queries(allocate_worst_fit, "worst fit");
    return 0;
}

void run_queries(void (*allocate)(unsigned int, int), const char* name) {
    memset(arr, 0, sizeof(arr));
    
    char cmnd[20];
    unsigned int adrs;
    int size;
    int queries = 0;

    FILE* file = fopen("queries.txt", "r");
    if (file == NULL) return;

    clock_t start = clock();
    while (fscanf(file, "%s", cmnd) == 1) {
        if (strcmp(cmnd, "end") == 0) {
            break;
        }
        else if (strcmp(cmnd, "allocate") == 0) {
            fscanf(file, "%u %d", &adrs, &size);
            allocate(adrs, size);
            queries++;
        }
        else if (strcmp(cmnd, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
            queries++;
        }
    }
    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    fclose(file);
    printf("%s\n", name);
    printf("total queries executed: %d\n", queries);
    printf("total time taken: %f s\n", total_time);
    printf("throughput: %f q/s\n", queries / total_time);
}

void allocate_best_fit(unsigned int adrs, int size) {
    int min_block_size = N+1;
    int min_block_start = -1;

    int curr_block_size = 0;
    int curr_block_start = -1;

    for (int i = 0; i < N; ++i) {

        // meet non-busy block (hole)
        if (arr[i] == 0) {
            curr_block_size++;
            if (curr_block_start == -1) {
                curr_block_start = i;
            }
        }

        // when we meet a busy block, we check for the 
        // size of the previous block (hole)
        // and update min values if needed
        else {
            if (curr_block_size == size) {
                for (int j = curr_block_start; j < curr_block_start + size; ++j) {
                    arr[j] = adrs;
                }
                return;
            }
            if (curr_block_size > size && curr_block_size < min_block_size) {
                min_block_size = curr_block_size;
                min_block_start = curr_block_start;
            }
            curr_block_size = 0;
            curr_block_start = -1;
        }
    }

    // check in case the smallest hole is at the end
        if (curr_block_size >= size && curr_block_size < min_block_size) {
            min_block_size = curr_block_size;
            min_block_start = curr_block_start;
        }

    // filling
    if (min_block_start != -1 && min_block_size >= size) {
        for (int i = min_block_start; i < min_block_start + size; ++i) {
            arr[i] = adrs;
        }
    }

}

void allocate_first_fit(unsigned int adrs, int size) {
    int curr_block_size = 0;
    int curr_block_start = -1;

    for (int i = 0; i < N; ++i) {

        // meet non-busy block (hole)
        // and with each step check if the hole is big enough to allocate
        if (arr[i] == 0) {
            if (curr_block_size == 0) {
                curr_block_start = i;
            }
            curr_block_size++;

            if (curr_block_size == size) {
                for (int j = curr_block_start; j < curr_block_start + size; ++j) {
                    arr[j] = adrs;
                }
                return;
            }
        }

        // when meet a busy block, we start again
        // to find a hole
        else {
            curr_block_size = 0;
        }
    }
}

void allocate_worst_fit(unsigned int adrs, int size) {
    int max_block_size = -1;
    int max_block_start = -1;

    int curr_block_size = 0;
    int curr_block_start = -1;

    for (int i = 0; i < N; ++i) {

        // meet non-busy block (hole)
        if (arr[i] == 0) {
            curr_block_size++;
            if (curr_block_start == -1) {
                curr_block_start = i;
            }
        }

        // when we meet a busy block, we check for the 
        // size of the previous block (hole)
        // and update max values if needed
        else {
            if (curr_block_size > max_block_size) {
                max_block_size = curr_block_size;
                max_block_start = curr_block_start;
            }
            curr_block_size = 0;
            curr_block_start = -1;
        }
    }

    // check in case the largest hole is at the end
        if (curr_block_size > max_block_size) {
            max_block_size = curr_block_size;
            max_block_start = curr_block_start;
        }

    // filling
    if (max_block_start != -1 && max_block_size >= size) {
        for (int i = max_block_start; i < max_block_start + size; ++i) {
            arr[i] = adrs;
        }
    }

}

void clear(unsigned int adrs) {
    if (adrs == 0) return;
    for (int i = 0; i < N; ++i) {
        if (arr[i] == adrs) {
            arr[i] = 0;
        }
    }
}