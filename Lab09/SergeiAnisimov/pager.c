/* Lab 09 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include "paging.h"
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

enum { PAGE_BYTES = 8 };
enum Policy { RANDOM, NFU, AGING };

static int random_victim(const struct PTE *table, int pages)
{
    int chosen = -1;
    unsigned int candidates = 0;
    for (int i = 0; i < pages; ++i) {
        if (table[i].valid && (unsigned int)rand() % ++candidates == 0)
            chosen = i;
    }
    return chosen;
}

static int nfu(const struct PTE *table, int pages)
{
    int chosen = -1;
    for (int i = 0; i < pages; ++i) {
        if (table[i].valid && (chosen == -1 || table[i].frequency < table[chosen].frequency))
            chosen = i;
    }
    return chosen;
}

static int aging(const struct PTE *table, int pages)
{
    int chosen = -1;
    for (int i = 0; i < pages; ++i) {
        if (table[i].valid && (chosen == -1 || table[i].age < table[chosen].age))
            chosen = i;
    }
    return chosen;
}

static void print_ram(char (*ram)[PAGE_BYTES], int frames)
{
    for (int i = 0; i < frames; ++i)
        printf("Frame %d ---> %s\n", i, ram[i]);
}

static int run_pager(struct PTE *table, int pages, int frames, enum Policy policy,
                     const sigset_t *signals, char (*disk)[PAGE_BYTES],
                     char (*ram)[PAGE_BYTES])
{
    int (*const choose[])(const struct PTE *, int) = {random_victim, nfu, aging};
    int free_frame = 0;
    size_t accesses = 0;

    puts("Initialized page table");
    print_table(table, pages);
    puts("Initialized RAM");
    print_ram(ram, frames);
    puts("Initialized disk");
    for (int i = 0; i < pages; ++i)
        printf("Page %d ---> %s\n", i, disk[i]);

    printf("Pager ready (PID=%ld)\n", (long)getpid());
    fflush(stdout);
    pid_t mmu = 0;

    for (;;) {
        siginfo_t info;
        int received = sigwaitinfo(signals, &info);
        if (received == -1) {
            if (errno == EINTR)
                continue;
            perror("sigwaitinfo");
            return 1;
        }
        if (received != SIGUSR1)
            break;
        if (mmu != 0 && mmu != info.si_pid)
            continue;

        int page = 0;
        while (page < pages && table[page].referenced != info.si_pid)
            ++page;
        if (page == pages) {
            if (mmu == info.si_pid)
                break;
            continue;
        }

        mmu = info.si_pid;
        printf("Page fault for page %d from MMU %ld\n", page, (long)mmu);
        int frame = free_frame;
        if (free_frame < frames) {
            ++free_frame;
            printf("Allocating free frame %d\n", frame);
        } else {
            int victim = choose[policy](table, pages);
            frame = table[victim].frame;
            printf("Evicting page %d from frame %d\n", victim, frame);
            if (table[victim].dirty) {
                memcpy(disk[victim], ram[frame], PAGE_BYTES);
                printf("Writing dirty page %d to disk\n", victim);
                ++accesses;
                print_ram(ram, frames);
            }
            table[victim] = (struct PTE){.frame = -1};
        }

        memcpy(ram[frame], disk[page], PAGE_BYTES);
        ++accesses;
        table[page] = (struct PTE){.valid = true, .frame = frame};
        print_ram(ram, frames);
        print_table(table, pages);
        fflush(stdout);
        if (kill(mmu, SIGCONT) == -1) {
            perror("SIGCONT");
            return 1;
        }
    }

    printf("Total disk accesses: %zu\n", accesses);
    return 0;
}

int main(int argc, char *argv[])
{
    int pages = argc >= 3 ? read_number(argv[1], 1) : -1;
    int frames = argc >= 3 ? read_number(argv[2], 1) : -1;
    enum Policy policy = RANDOM;
    bool valid_policy = true;
    if (argc == 4) {
        if (strcmp(argv[3], "nfu") == 0)
            policy = NFU;
        else if (strcmp(argv[3], "aging") == 0)
            policy = AGING;
        else if (strcmp(argv[3], "random") != 0)
            valid_policy = false;
    }
    if ((argc != 3 && argc != 4) || pages < 1 || frames < 1 || frames > pages ||
        !valid_policy || (size_t)pages > SIZE_MAX / sizeof(struct PTE)) {
        fprintf(stderr, "Usage: %s pages frames [random|nfu|aging]\n", argv[0]);
        return 1;
    }

    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGTERM);
    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    const char *path = table_path();
    if (strcmp(path, "/tmp/ex2/pagetable") == 0 &&
        mkdir("/tmp/ex2", 0700) == -1 && errno != EEXIST) {
        perror("/tmp/ex2");
        return 1;
    }

    int fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd == -1) {
        perror(path);
        return 1;
    }
    size_t length = (size_t)pages * sizeof(struct PTE);
    struct PTE *table = MAP_FAILED;
    if (ftruncate(fd, (off_t)length) == 0)
        table = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int error = errno;
    close(fd);
    if (table == MAP_FAILED) {
        errno = error;
        perror("page table");
        unlink(path);
        return 1;
    }

    char (*disk)[PAGE_BYTES] = calloc((size_t)pages, sizeof *disk);
    char (*ram)[PAGE_BYTES] = calloc((size_t)frames, sizeof *ram);
    int result = 1;
    if (disk && ram) {
        for (int i = 0; i < pages; ++i) {
            table[i] = (struct PTE){.frame = -1};
            snprintf(disk[i], PAGE_BYTES, "%07x", (unsigned int)i & 0xfffffff);
        }
        printf("Page replacement: %s\n", argc == 4 ? argv[3] : "random");
        result = run_pager(table, pages, frames, policy, &signals, disk, ram);
    } else {
        perror("calloc");
    }
    free(ram);
    free(disk);
    munmap(table, length);
    unlink(path);
    return result;
}
