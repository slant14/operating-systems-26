// best fit 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 10000000

void allocate_best_fit(unsigned int adrs, int size);
void clear(unsigned int adrs);

unsigned int arr[N] = {0};

int main()
{
    char cmnd[20];
    unsigned int adrs;
    int size;
    int queries = 0;

    FILE* file = fopen("queries.txt", "r");
    if (file == NULL) return 1;

    clock_t start = clock();
    while (fscanf(file, "%s", cmnd) == 1) {
        if (strcmp(cmnd, "end") == 0) {
            break;
        }
        else if (strcmp(cmnd, "allocate") == 0) {
            fscanf(file, "%u %d", &adrs, &size);
            allocate_best_fit(adrs, size);
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
    printf("best fit\n");
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

void clear(unsigned int adrs) {
    if (adrs == 0) return;
    for (int i = 0; i < N; ++i) {
        if (arr[i] == adrs) {
            arr[i] = 0;
        }
    }
}