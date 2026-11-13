/* Lab 07 Sergei Anisimov CSE-04 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef MEMORY_SIZE
#define MEMORY_SIZE 10000000u
#endif

static unsigned int memory[MEMORY_SIZE];

enum Fit { FIRST_FIT, BEST_FIT, WORST_FIT };

/* Return the start of a free block, or MEMORY_SIZE if none fits. */
static size_t find_block(size_t needed, enum Fit fit)
{
    size_t choice = MEMORY_SIZE;
    size_t choice_size = (fit == BEST_FIT) ? MEMORY_SIZE + 1u : 0u;

    for (size_t i = 0; i < MEMORY_SIZE; ) {
        if (memory[i] != 0) {
            i++;
            continue;
        }

        size_t start = i;
        while (i < MEMORY_SIZE && memory[i] == 0) {
            i++;
            if (fit == FIRST_FIT && i - start == needed)
                return start;
        }

        size_t length = i - start;
        if (length < needed)
            continue;

        if ((fit == BEST_FIT && length < choice_size) ||
            (fit == WORST_FIT && length > choice_size)) {
            choice = start;
            choice_size = length;
        }
    }
    return choice;
}

static void allocate(unsigned int adrs, size_t size, enum Fit fit)
{
    if (adrs == 0 || size == 0 || size > MEMORY_SIZE)
        return;

    size_t start = find_block(size, fit);
    if (start == MEMORY_SIZE)
        return;

    for (size_t i = 0; i < size; i++)
        memory[start + i] = adrs;
}

void allocate_first_fit(unsigned int adrs, size_t size)
{
    allocate(adrs, size, FIRST_FIT);
}

void allocate_best_fit(unsigned int adrs, size_t size)
{
    allocate(adrs, size, BEST_FIT);
}

void allocate_worst_fit(unsigned int adrs, size_t size)
{
    allocate(adrs, size, WORST_FIT);
}

void clear(unsigned int adrs)
{
    if (adrs == 0)
        return;

    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs)
            memory[i] = 0;
    }
}

static int run_test(const char *name,
                    void (*allocate_function)(unsigned int, size_t))
{
    FILE *file = fopen("queries.txt", "r");
    if (file == NULL) {
        perror("queries.txt");
        return 1;
    }

    memset(memory, 0, sizeof memory);

    char command[16];
    unsigned int adrs;
    size_t size;
    size_t count = 0;
    int error = 0;
    clock_t start = clock();
    clock_t finish = start;

    while (fscanf(file, "%15s", command) == 1) {
        if (strcmp(command, "end") == 0)
            break;

        if (strcmp(command, "allocate") == 0) {
            if (fscanf(file, "%u %zu", &adrs, &size) != 2 ||
                adrs == 0 || size == 0 || size > MEMORY_SIZE) {
                error = 1;
                break;
            }
            allocate_function(adrs, size);
        } else if (strcmp(command, "clear") == 0) {
            if (fscanf(file, "%u", &adrs) != 1 || adrs == 0) {
                error = 1;
                break;
            }
            clear(adrs);
        } else {
            error = 1;
            break;
        }
        count++;
        finish = clock();
    }

    if (ferror(file))
        error = 1;
    fclose(file);

    if (error) {
        fprintf(stderr, "Invalid query in queries.txt\n");
        return 1;
    }
    if (start == (clock_t)-1 || finish == (clock_t)-1) {
        fputs("CPU time is unavailable.\n", stderr);
        return 1;
    }

    double seconds = (double)(finish - start) / CLOCKS_PER_SEC;
    double throughput = seconds > 0.0 ? count / seconds : 0.0;
    printf("%s: %zu queries, %.6f seconds, %.2f queries/second\n",
           name, count, seconds, throughput);
    return 0;
}

int main(void)
{
    if (run_test("First Fit", allocate_first_fit) != 0)
        return 1;
    if (run_test("Best Fit", allocate_best_fit) != 0)
        return 1;
    if (run_test("Worst Fit", allocate_worst_fit) != 0)
        return 1;
    return 0;
}
