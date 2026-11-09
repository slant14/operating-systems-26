#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SIZE 120

int main(void) {
    int u[SIZE], v[SIZE];
    int n;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < SIZE; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    FILE *file = fopen("temp.txt", "w");
    if (!file) return 1;
    fclose(file);

    int chunk = SIZE / n;

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) return 1;

        if (pid == 0) {
            long long part_sum = 0;
            int start_idx = i * chunk;
            int end_idx = start_idx + chunk;

            for (int j = start_idx; j < end_idx; j++) {
                part_sum += (long long)u[j] * v[j];
            }

            file = fopen("temp.txt", "a");
            if (!file) exit(1);
            fprintf(file, "%lld\n", part_sum);
            fclose(file);

            exit(0);
        }
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    file = fopen("temp.txt", "r");
    if (!file) return 1;

    long long total_sum = 0;
    long long val = 0;

    while (fscanf(file, "%lld", &val) == 1) {
        total_sum += val;
    }
    fclose(file);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%lld\n", total_sum);
    printf("%.6f\n", elapsed_time);

    return 0;
}