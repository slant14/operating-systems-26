/* Lab 09 Sergei Anisimov CSE-04 */
#ifndef PAGING_H
#define PAGING_H

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    unsigned int frequency;
    uint8_t age;
};

static inline int read_number(const char *text, int minimum)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (errno || end == text || *end || value < minimum || value > INT_MAX)
        return -1;
    return (int)value;
}

static inline const char *table_path(void)
{
    const char *override = getenv("OS_PAGE_TABLE");
    return override && *override ? override : "/tmp/ex2/pagetable";
}

static inline void print_table(const struct PTE *table, int pages)
{
    for (int i = 0; i < pages; ++i)
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, table[i].valid, table[i].frame, table[i].dirty, table[i].referenced);
}
#endif
