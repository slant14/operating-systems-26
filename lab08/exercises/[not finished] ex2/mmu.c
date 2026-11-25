// [Not Finished]

#include <stdio.h>
#include "pte.h"

void mmu(void) {
    FILE* pagetable = fopen("/tmp/ex2/pagetable", "r");
    if (pagetable == NULL) return 1;

    char page = read(pagetable);
    if (page.valid != 0) continue;
    if (page.valid == 0) {
        page.valid = mmu.valid;
        signal(SIGUSR1, func1);

        while !(signal(SIGCONT, default)) {
            pause;
        }
    }
    
    if (strcmpr(pagetable.access, "w")) {

    }
}