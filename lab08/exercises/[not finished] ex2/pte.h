#indlude <stdbool.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};