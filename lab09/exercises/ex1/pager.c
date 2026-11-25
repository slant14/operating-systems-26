#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "pte.h"

PTE* page_table;
char (*disk)[8];
char (*ram)[8];
int pages;
int frames;
int used = 0;
int disk_accesses = 0;

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

void print_ram()
{
    printf("RAM array\n");
    for (int i = 0; i < frames; i++)
        printf("Frame %d | %s\n", i, ram[i]);
}

// create shared page table
int create_table()
{
    if (mkdir("/tmp/ex2", 0700) == -1 && errno != EEXIST) {
        perror("mkdir");
        return -1;
    }

    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    size_t size = (size_t)pages * sizeof(PTE);
    if (ftruncate(fd, (off_t)size) == -1) {
        perror("ftruncate");
        close(fd);
        unlink("/tmp/ex2/pagetable");
        return -1;
    }

    page_table = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        unlink("/tmp/ex2/pagetable");
        return -1;
    }

    for (int i = 0; i < pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
        page_table[i].nfu_counter = 0;
        page_table[i].age = 0;
    }
    return 0;
}

// choose page to replace
int random_page(void)
{
    int candidates[pages];
    int count = 0;
    for (int i = 0; i < pages; i++) {
        if (page_table[i].valid)
            candidates[count++] = i;
    }
    if (count == 0) return -1;
    return candidates[rand() % count];
}

int nfu(void)
{
    int chosen = -1;
    for (int i = 0; i < pages; i++) {
        if (!page_table[i].valid) continue;
        if (chosen == -1 || page_table[i].nfu_counter < page_table[chosen].nfu_counter)
            chosen = i;
    }
    return chosen;
}

int aging(void)
{
    int chosen = -1;
    for (int i = 0; i < pages; i++) {
        if (!page_table[i].valid) continue;
        if (chosen == -1 || page_table[i].age < page_table[chosen].age)
            chosen = i;
    }
    return chosen;
}

// load one page from disk to ram
int load_page(int page, int (*algorithm)(void))
{
    int frame;
    if (used < frames) {
        frame = used++;
        printf("Allocate free frame %d\n", frame);
    } else {
        int victim = algorithm();
        if (victim == -1) {
            printf("No page to replace\n");
            return -1;
        }

        frame = page_table[victim].frame;
        printf("Evict page %d from frame %d\n", victim, frame);

        if (page_table[victim].dirty) {
            strcpy(disk[victim], ram[frame]);
            disk_accesses++;
            printf("Write dirty page %d to disk\n", victim);
        }

        page_table[victim].valid = false;
        page_table[victim].frame = -1;
        page_table[victim].dirty = false;
        page_table[victim].nfu_counter = 0;
        page_table[victim].age = 0;
    }

    strcpy(ram[frame], disk[page]);
    disk_accesses++;
    page_table[page].valid = true;
    page_table[page].frame = frame;
    page_table[page].dirty = false;
    page_table[page].referenced = 0;
    page_table[page].nfu_counter = 0;
    page_table[page].age = 0;

    printf("Copy page %d from disk to RAM frame %d\n", page, frame);
    print_ram();
    print_table();
    printf("Disk accesses so far: %d\n", disk_accesses);
    fflush(stdout);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Usage: ./pager random|nfu|aging pages frames\n");
        return 1;
    }

    pages = atoi(argv[2]);
    frames = atoi(argv[3]);
    int (*algorithm)(void) = NULL;
    if (strcmp(argv[1], "random") == 0) algorithm = random_page;
    if (strcmp(argv[1], "nfu") == 0) algorithm = nfu;
    if (strcmp(argv[1], "aging") == 0) algorithm = aging;

    if (pages <= 0 || frames <= 0 || frames > pages || algorithm == NULL) {
        printf("Invalid arguments\n");
        return 1;
    }

    // wait for sigusr1
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    if (create_table() == -1) return 1;

    // create disk and ram
    disk = calloc(pages, sizeof(*disk));
    ram = calloc(frames, sizeof(*ram));
    if (disk == NULL || ram == NULL) {
        perror("calloc");
        free(disk);
        free(ram);
        munmap(page_table, (size_t)pages * sizeof(PTE));
        unlink("/tmp/ex2/pagetable");
        return 1;
    }
    for (int i = 0; i < pages; i++)
        snprintf(disk[i], 8, "P%06d", i % 1000000);

    srand(1);
    printf("Pager PID: %d\n", getpid());
    printf("Selected algorithm: %s\n", argv[1]);
    print_table();
    print_ram();
    fflush(stdout);

    int result = 0;
    while (1) {
        int signal_number;
        int error = sigwait(&signals, &signal_number);
        if (error != 0) {
            fprintf(stderr, "sigwait failed: %s\n", strerror(error));
            result = 1;
            break;
        }

        int page = -1;
        for (int i = 0; i < pages; i++) {
            if (page_table[i].referenced != 0) {
                page = i;
                break;
            }
        }
        if (page == -1) break; // mmu finished all requests

        int pid = page_table[page].referenced;
        printf("Page %d is referenced\n", page);
        if (load_page(page, algorithm) == -1) {
            result = 1;
            break;
        }
        if (kill(pid, SIGCONT) == -1) {
            perror("kill");
            result = 1;
            break;
        }
    }

    printf("Total disk accesses: %d\n", disk_accesses);
    munmap(page_table, (size_t)pages * sizeof(PTE));
    unlink("/tmp/ex2/pagetable");
    free(disk);
    free(ram);
    return result;
}
