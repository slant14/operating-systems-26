/* Lab 09 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include "paging.h"
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

struct TLBEntry { int page; int frame; };

static int page_of(const char *request, int pages)
{
    if ((request[0] != 'R' && request[0] != 'W') ||
        request[1] < '0' || request[1] > '9')
        return -1;
    int page = read_number(request + 1, 0);
    return page < pages ? page : -1;
}

static int find_entry(const struct TLBEntry *cache, int entries, int page)
{
    for (int i = 0; i < entries; ++i)
        if (cache[i].page == page)
            return i;
    return -1;
}

static void cache_mapping(struct TLBEntry *cache, int entries, int position,
                          int page, int frame)
{
    if (entries == 0)
        return;
    if (position < 0)
        position = entries - 1;
    memmove(cache + 1, cache, (size_t)position * sizeof *cache);
    cache[0] = (struct TLBEntry){page, frame};
}

static void invalidate_evicted(struct TLBEntry *cache, int entries, const struct PTE *table)
{
    int kept = 0;
    for (int i = 0; i < entries; ++i) {
        int page = cache[i].page;
        if (page >= 0 && table[page].valid && table[page].frame == cache[i].frame)
            cache[kept++] = cache[i];
    }
    while (kept < entries)
        cache[kept++] = (struct TLBEntry){-1, -1};
}

static int load_page(struct PTE *pte, pid_t pager, const sigset_t *signals)
{
    pte->referenced = getpid();
    if (kill(pager, SIGUSR1) == -1) {
        perror("SIGUSR1");
        return 1;
    }

    for (;;) {
        siginfo_t info;
        if (sigwaitinfo(signals, &info) == -1) {
            if (errno == EINTR)
                continue;
            perror("sigwaitinfo");
            return 1;
        }
        if (info.si_pid == pager && pte->valid)
            return 0;
    }
}

static int process_requests(struct PTE *table, int pages, pid_t pager, const sigset_t *signals,
                            int requests, char *argv[], struct TLBEntry *cache, int entries)
{
    int hits = 0;
    int tlb_misses = 0;
    for (int i = 0; i < requests; ++i) {
        int page = page_of(argv[i], pages);
        int entry = find_entry(cache, entries, page);
        int frame;
        printf("%s request for page %d\n", argv[i][0] == 'R' ? "Read" : "Write", page);
        if (entry >= 0) {
            frame = cache[entry].frame;
            ++hits;
        } else {
            ++tlb_misses;
            if (table[page].valid) {
                ++hits;
            } else {
                puts("Page fault");
                fflush(stdout);
                if (load_page(&table[page], pager, signals) != 0)
                    return 1;
                invalidate_evicted(cache, entries, table);
            }
            frame = table[page].frame;
        }

        cache_mapping(cache, entries, entry, page, frame);
        if (argv[i][0] == 'W')
            table[page].dirty = true;
        for (int p = 0; p < pages; ++p)
            table[p].age >>= 1;
        table[page].age |= UINT8_C(128);
        if (table[page].frequency < UINT_MAX)
            ++table[page].frequency;

        printf("Frame: %d\n", frame);
        print_table(table, pages);
    }

    printf("Page table hit ratio: %.2f%% (%d/%d)\n", 100.0 * hits / requests, hits, requests);
    if (entries > 0)
        printf("TLB miss ratio: %.2f%% (%d/%d)\n", 100.0 * tlb_misses / requests,
               tlb_misses, requests);
    return 0;
}

int main(int argc, char *argv[])
{
    bool use_tlb = argc > 1 && strcmp(argv[1], "--tlb") == 0;
    int first = use_tlb ? 2 : 1;
    int pages = argc >= first + 3 ? read_number(argv[first], 1) : -1;
    int pager = argc >= first + 3 ? read_number(argv[argc - 1], 2) : -1;
    if (pages < 1 || pager < 2 || (size_t)pages > SIZE_MAX / sizeof(struct PTE)) {
        fprintf(stderr, "Usage: %s [--tlb] pages R0 W1 ... pager_pid\n", argv[0]);
        return 1;
    }

    for (int i = first + 1; i < argc - 1; ++i) {
        if (page_of(argv[i], pages) < 0) {
            fprintf(stderr, "Invalid memory request: %s\n", argv[i]);
            return 1;
        }
    }

    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGCONT);
    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    int fd = open(table_path(), O_RDWR);
    if (fd == -1) {
        perror(table_path());
        return 1;
    }
    struct stat info;
    size_t length = (size_t)pages * sizeof(struct PTE);
    if (fstat(fd, &info) == -1 || !S_ISREG(info.st_mode) ||
        info.st_size != (off_t)length) {
        fputs("Page table file has the wrong size or type.\n", stderr);
        close(fd);
        return 1;
    }
    struct PTE *table = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (table == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    int entries = use_tlb ? (pages < 5 ? 1 : pages / 5) : 0;
    struct TLBEntry *cache = entries ? malloc((size_t)entries * sizeof *cache) : NULL;
    int result = 1;
    if (entries == 0 || cache) {
        for (int i = 0; i < entries; ++i)
            cache[i] = (struct TLBEntry){-1, -1};
        result = process_requests(table, pages, pager, &signals, argc - first - 2,
                                  argv + first + 1, cache, entries);
    } else {
        perror("malloc");
    }
    free(cache);
    munmap(table, length);
    if (result == 0 && kill(pager, SIGUSR1) == -1) {
        perror("finishing pager");
        return 1;
    }
    return result;
}
