#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// #define NUM 120
#define NUM 1000000
int u[NUM], v[NUM];

int main()
{
    int length = sizeof(u) / sizeof(u[0]);

    for (int i = 0; i < length; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    int n;
    scanf("%d", &n);

    // how many processes for each child
    int total = NUM / n;
    long long total_sum = 0;
    long long local_sum = 0;

    // clear file
    FILE* clear_file = fopen("temp.txt", "w");
    if (clear_file != NULL) fclose(clear_file);

    clock_t start_time = clock();

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) return EXIT_FAILURE;
        
        // child
        if (pid == 0) {
            pid = i;

            // child should make process with such ids
            int start = i*total;
            int end = i*total + total - 1;

            for (int s = start; s <= end; s++) {
                local_sum += u[s] * v[s];
            }

            FILE* file = fopen("temp.txt", "a");
                if (file != NULL) {
                    fprintf(file, "%lld\n", local_sum);
                }
            fclose(file);
            exit(0);
        }
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    FILE* file = fopen("temp.txt", "r");
        if (file != NULL) {
            for (int i = 0; i < n; i++) {
                fscanf(file, "%lld", &local_sum);
                total_sum += local_sum;
            }
        }
    fclose(file);
    
    clock_t end_time = clock();

    printf("%lld\n", total_sum);
    printf("the total time: %lld\n", end_time - start_time);
}