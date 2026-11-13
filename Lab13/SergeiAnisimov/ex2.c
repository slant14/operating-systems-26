/* Lab 13 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_data(FILE *file, unsigned long long **data,
                     size_t *rows, size_t *columns)
{
    char *line = NULL;
    size_t capacity = 0;
    int result = 0;
    while (getline(&line, &capacity, file) != -1) {
        char *cursor;
        char *token = strtok_r(line, " \t\r\n", &cursor);
        size_t count = 0;
        while (token) {
            if (token[0] < '0' || token[0] > '9') {
                result = 1;
                break;
            }
            errno = 0;
            char *end;
            unsigned long long value = strtoull(token, &end, 10);
            if (errno || *end || (*columns && count >= *columns) ||
                *rows > (SIZE_MAX / sizeof **data - count - 1) / (*columns ? *columns : 1)) {
                result = 1;
                break;
            }
            size_t index = *rows * *columns + count;
            unsigned long long *values = realloc(*data, (index + 1) * sizeof **data);
            if (!values) {
                perror("realloc");
                result = 1;
                break;
            }
            *data = values;
            values[index] = value;
            ++count;
            token = strtok_r(NULL, " \t\r\n", &cursor);
        }
        if (result || (count && *columns && count != *columns)) {
            result = 1;
            break;
        }
        if (count) {
            *columns = count;
            ++*rows;
        }
    }
    result |= ferror(file) != 0;
    free(line);
    return result;
}

static bool allocation_is_valid(const unsigned long long *total,
                                const unsigned long long *available,
                                const unsigned long long *allocated,
                                size_t processes, size_t columns)
{
    for (size_t resource = 0; resource < columns; ++resource) {
        unsigned long long remaining = total[resource];
        for (size_t process = 0; process < processes; ++process) {
            unsigned long long held = allocated[process * columns + resource];
            if (held > remaining)
                return false;
            remaining -= held;
        }
        if (remaining != available[resource])
            return false;
    }
    return true;
}

static int check_state(unsigned long long *values, size_t rows, size_t columns)
{
    if (rows < 4 || (rows - 2) % 2 != 0) {
        fputs("Expected two vectors followed by equally sized allocation and request matrices.\n", stderr);
        return 1;
    }
    size_t processes = (rows - 2) / 2;
    unsigned long long *available = values + columns;
    const unsigned long long *allocated = available + columns;
    const unsigned long long *requests = allocated + processes * columns;
    if (!allocation_is_valid(values, available, allocated, processes, columns)) {
        fputs("The resource totals do not match the allocation and availability.\n", stderr);
        return 1;
    }
    bool *finished = calloc(processes, sizeof *finished);
    if (!finished) {
        perror("calloc");
        return 1;
    }
    size_t remaining = processes;
    bool progress;
    do {
        progress = false;
        for (size_t p = 0; p < processes; ++p) {
            if (finished[p])
                continue;
            size_t r = 0;
            while (r < columns && requests[p * columns + r] <= available[r])
                ++r;
            if (r == columns) {
                printf("Process %zu can finish\n", p + 1);
                for (r = 0; r < columns; ++r)
                    available[r] += allocated[p * columns + r];
                finished[p] = progress = true;
                --remaining;
            }
        }
    } while (progress && remaining > 0);

    if (remaining == 0) {
        puts("No deadlock detected");
    } else {
        printf("Processes that cannot finish:");
        for (size_t p = 0; p < processes; ++p)
            if (!finished[p])
                printf(" %zu", p + 1);
        putchar('\n');
    }
    free(finished);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [input.txt]\n", argv[0]);
        return 2;
    }
    const char *path = argc == 2 ? argv[1] : "input.txt";
    FILE *file = fopen(path, "r");
    if (!file) {
        perror(path);
        return 1;
    }
    unsigned long long *values = NULL;
    size_t rows = 0, columns = 0;
    int result = read_data(file, &values, &rows, &columns);
    fclose(file);
    if (result)
        fputs("Invalid input matrix.\n", stderr);
    else
        result = check_state(values, rows, columns);
    free(values);
    return result;
}
