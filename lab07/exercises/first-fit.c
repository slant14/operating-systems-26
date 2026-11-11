// first fit 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 10000000

void allocate_first_fit(unsigned int adrs, int size);
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
            allocate_first_fit(adrs, size);
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
    printf("first fit\n");
    printf("total queries executed: %d\n", queries);
    printf("total time taken: %f s\n", total_time);
    printf("throughput: %f q/s\n", queries / total_time);
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

void clear(unsigned int adrs) {
    if (adrs == 0) return;
    for (int i = 0; i < N; ++i) {
        if (arr[i] == adrs) {
            arr[i] = 0;
        }
    }
}