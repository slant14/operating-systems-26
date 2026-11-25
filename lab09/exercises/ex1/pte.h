#include <stdbool.h>

typedef struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int nfu_counter;
    unsigned char age;
} PTE;