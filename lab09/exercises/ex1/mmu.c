#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "pte.h"

PTE* page_table;
int pages;
int pager_pid;
int hits = 0;
int misses = 0;
sigset_t signals;

void print_table()
{
    printf("Page table\n");
    for (int i = 0; i < pages; i++) {
        printf("Page %d | valid=%d, frame=%d, dirty=%d, referenced=%d, nfu=%d, age=%u\n",
               i, page_table[i].valid, page_table[i].frame,
               page_table[i].dirty, page_table[i].referenced,
               page_table[i].nfu_counter, page_table[i].age);
    }
}

// open shared page table
int open_table()
{
    int fd = open("/tmp/ex2/pagetable", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    size_t size = (size_t)pages * sizeof(PTE);
    struct stat info;
    if (fstat(fd, &info) == -1) {
        perror("fstat");
        close(fd);
        return -1;
    }
    if (info.st_size != (off_t)size) {
        printf("Page table size does not match\n");
        close(fd);
        return -1;
    }

    page_table = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    return 0;
}

// update counters after every request
void update_counters(int page)
{
    page_table[page].nfu_counter++;

    for (int i = 0; i < pages; i++) {
        if (page_table[i].valid) {
            page_table[i].age = page_table[i].age >> 1;

            if (i == page) {
                page_table[i].age |= 128; // set the MSB to 1 - 10000000
            }
        }
    }
}

// read or write a page
int access_page(int page, char mode)
{
    printf("-------------------------\n");
    printf("%s request for page %d\n", mode == 'W' ? "Write" : "Read", page);

    if (page_table[page].valid) {
        hits++;
        printf("Page hit\n");
    } else {
        misses++;
        printf("Page fault\n");
        page_table[page].referenced = getpid();
        printf("Ask pager to load page %d\n", page);
        fflush(stdout);

        if (kill(pager_pid, SIGUSR1) == -1) {
            perror("kill");
            return -1;
        }
        int signal_number;
        int error = sigwait(&signals, &signal_number);
        if (error != 0) {
            fprintf(stderr, "sigwait failed: %s\n", strerror(error));
            return -1;
        }
        printf("MMU resumed by SIGCONT\n");
    }

    if (mode == 'W') {
        page_table[page].dirty = true;
        printf("Set dirty field for page %d\n", page);
    }
    update_counters(page);
    print_table();
    fflush(stdout);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("Usage: ./mmu pages R0 W1 ... pager_pid\n");
        return 1;
    }

    pages = atoi(argv[1]);
    pager_pid = atoi(argv[argc - 1]);
    if (pages <= 0 || pager_pid <= 0) {
        printf("Invalid arguments\n");
        return 1;
    }

    // wait for sigcont
    sigemptyset(&signals);
    sigaddset(&signals, SIGCONT);
    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    if (open_table() == -1) return 1;
    printf("MMU process\n");
    print_table();

    int result = 0;
    for (int i = 2; i < argc - 1; i++) {
        char mode = argv[i][0];
        char* end;
        errno = 0;
        long page = strtol(argv[i] + 1, &end, 10);
        if ((mode != 'R' && mode != 'W') || argv[i][1] == '\0' ||
            errno != 0 || *end != '\0' || page < 0 || page >= pages) {
            printf("Invalid request: %s\n", argv[i]);
            result = 1;
            break;
        }
        if (access_page((int)page, mode) == -1) {
            result = 1;
            break;
        }
    }

    if (result == 0) {
        printf("Done all requests\n");
        printf("Hits: %d\n", hits);
        printf("Misses: %d\n", misses);
        printf("Hit ratio: %.4f\n", (double)hits / (hits + misses));
    }

    // tell pager that mmu is finished
    printf("MMU sends SIGUSR1 to pager\n");
    fflush(stdout);
    if (kill(pager_pid, SIGUSR1) == -1) {
        perror("kill");
        result = 1;
    }
    if (munmap(page_table, (size_t)pages * sizeof(PTE)) == -1) {
        perror("munmap");
        result = 1;
    }
    return result;
}
