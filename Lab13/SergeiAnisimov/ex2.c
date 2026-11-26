/* Lab 13 Exercise 2 | Sergei Anisimov, CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long Units;

static bool append_value(Units **values, size_t used, size_t *capacity, Units value)
{
    if (used == *capacity) {
        const size_t limit = SIZE_MAX / sizeof **values;
        if (*capacity == limit)
            return false;
        size_t next = *capacity == 0 ? 32 : *capacity * 2;
        if (*capacity > limit / 2)
            next = limit;
        Units *larger = realloc(*values, next * sizeof **values);
        if (!larger)
            return false;
        *values = larger;
        *capacity = next;
    }
    (*values)[used] = value;
    return true;
}

static bool read_matrix(FILE *file, Units **values, size_t *rows, size_t *columns)
{
    char *line = NULL;
    size_t line_size = 0, used = 0, capacity = 0;
    bool valid = true;

    while (valid && getline(&line, &line_size, file) >= 0) {
        size_t width = 0;
        char *rest;
        for (char *word = strtok_r(line, " \t\r\n", &rest); word;
             word = strtok_r(NULL, " \t\r\n", &rest)) {
            char *end;
            errno = 0;
            Units value = strtoull(word, &end, 10);
            if (*word < '0' || *word > '9' || errno || *end ||
                !append_value(values, used, &capacity, value)) {
                valid = false;
                break;
            }
            ++used;
            ++width;
        }
        if (width == 0)
            continue;
        if (*columns == 0)
            *columns = width;
        else if (*columns != width)
            valid = false;
        ++*rows;
    }
    if (!feof(file) || ferror(file))
        valid = false;
    free(line);
    return valid && *rows >= 4 && (*rows - 2) % 2 == 0;
}

static bool valid_totals(const Units *total, const Units *available,
                         const Units *allocation, size_t processes, size_t resources)
{
    for (size_t r = 0; r < resources; ++r) {
        Units free_units = total[r];
        for (size_t p = 0; p < processes; ++p) {
            Units held = allocation[p * resources + r];
            if (held > free_units)
                return false;
            free_units -= held;
        }
        if (free_units != available[r])
            return false;
    }
    return true;
}

static void print_vector(const Units *values, size_t resources)
{
    for (size_t r = 0; r < resources; ++r)
        printf(" %llu", values[r]);
    putchar('\n');
}

static int check_state(Units *work, const Units *allocation, Units *need,
                       size_t processes, size_t resources, bool has_max)
{
    if (has_max) {
        for (size_t i = 0; i < processes * resources; ++i) {
            if (allocation[i] > need[i]) {
                fputs("Allocation exceeds Max.\n", stderr);
                return 1;
            }
            need[i] -= allocation[i];
        }
    }

    bool *finished = calloc(processes, sizeof *finished);
    size_t *order = calloc(processes, sizeof *order);
    if (!finished || !order) {
        perror("calloc");
        free(finished);
        free(order);
        return 1;
    }

    puts("Need:");
    for (size_t p = 0; p < processes; ++p) {
        printf("P%zu:", p);
        print_vector(need + p * resources, resources);
    }
    fputs("Initial available:", stdout);
    print_vector(work, resources);

    size_t completed = 0;
    size_t before;
    do {
        before = completed;
        for (size_t p = 0; p < processes; ++p) {
            if (finished[p])
                continue;
            size_t r = 0;
            while (r < resources && need[p * resources + r] <= work[r])
                ++r;
            if (r < resources)
                continue;

            /* A process returns its current allocation when it finishes. */
            for (r = 0; r < resources; ++r)
                work[r] += allocation[p * resources + r];

            finished[p] = true;
            order[completed++] = p;
            printf("P%zu finishes, available:", p);
            print_vector(work, resources);
        }
    } while (completed < processes && completed != before);

    if (completed == processes) {
        puts("No deadlock detected.");
        fputs("Safe sequence:", stdout);
        for (size_t i = 0; i < processes; ++i)
            printf("%sP%zu", i == 0 ? " " : " -> ", order[i]);
    } else {
        puts("Unsafe state. No complete safe sequence.");
        fputs("Processes that cannot finish:", stdout);
        for (size_t p = 0; p < processes; ++p)
            if (!finished[p])
                printf(" P%zu", p);
    }
    putchar('\n');
    free(order);
    free(finished);
    return 0;
}

int main(int argc, char **argv)
{
    bool has_max = true;
    int argument = 1;
    if (argc > 1 && strcmp(argv[1], "--need") == 0) {
        has_max = false;
        ++argument;
    }
    if (argc > argument + 1 || (argc > argument && argv[argument][0] == '-')) {
        fprintf(stderr, "Usage: %s [--need] [input-file]\n", argv[0]);
        return 2;
    }

    const char *path = argc > argument ? argv[argument] :
                       (has_max ? "input.txt" : "slides-input.txt");
    FILE *file = fopen(path, "r");
    if (!file) {
        perror(path);
        return 1;
    }
    Units *matrix = NULL;
    size_t rows = 0, resources = 0;
    bool valid = read_matrix(file, &matrix, &rows, &resources);
    fclose(file);
    if (!valid) {
        fputs("Expected two vectors and two equally sized matrices of nonnegative integers.\n",
              stderr);
        free(matrix);
        return 1;
    }

    size_t processes = (rows - 2) / 2;
    Units *work = matrix + resources;
    const Units *allocation = work + resources;
    Units *need = work + (processes + 1) * resources;
    if (!valid_totals(matrix, work, allocation, processes, resources)) {
        fputs("Total resources do not match allocation plus available resources.\n", stderr);
        free(matrix);
        return 1;
    }

    int status = check_state(work, allocation, need, processes, resources, has_max);
    free(matrix);
    return status;
}
