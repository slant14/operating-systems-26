#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#define N 120

int main(void) {
    static int u[N], v[N];
    for(int i = 0; i < N; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }
    int n;
    scanf("%d", &n);
    FILE *f = fopen("temp.txt", "w");
    fclose(f);
    int size = N / n;
    pid_t *pid;
    pid = malloc(n * sizeof(pid_t));
    long long dot = 0;
    double time = 0;
    for(int i = 0; i < n; i++) {
        pid[i] = fork();
        if(pid[i] < 0) {
            return EXIT_FAILURE;
}
        if(pid[i] == 0) {
            clock_t start = clock();
            for(int j = i * size; j < (i + 1) * size; j++) {
                dot += u[j] * v[j];
            }
            clock_t end = clock();
            time = (double)(end - start) / CLOCKS_PER_SEC;
            FILE *out = fopen("temp.txt", "a");
            fprintf(out, "%lld %lf\n", dot, time);
            fclose(out);
            return 0;
        }
    }
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    
    f = fopen("temp.txt", "r");
    long long result = 0;
    double result_time = 0;
    while (fscanf(f, "%lld %lf", &dot, &time) == 2) {
        result += dot;
        result_time += time;
    }
    fclose(f);
    printf("Dot product: u * v = %lld\n", result);
    printf("Result_time = %lf\n", result_time);
    
    return 0;
}