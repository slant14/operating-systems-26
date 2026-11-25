// [Not Finished]

// the job of the pager is to
// maintain a process’ use of RAM and the page table

#include <stdio.h>
#include "pte.h"
#define P 8

char disk[10];


int main(int argc, char *argv[])
{
    int pages = atoi(argv[1]);
    int frames = atoi(argv[2])
    
}

int main()
{
    int num_pages;
    scanf("%d", &num_pages);

    char *page_table = mmap(num_pages * sizeof(struct PTE));

    int pid;
    scanf("%d", &pid);
}