/* Lab 07 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef TEXT_SIZE
#define TEXT_SIZE (500L * 1024L * 1024L)
#endif

static int generate_text(const char *path)
{
    FILE *random = fopen("/dev/random", "rb");
    if (random == NULL) {
        perror("/dev/random");
        return -1;
    }
    FILE *output = fopen(path, "wb");
    if (output == NULL) {
        perror(path);
        fclose(random);
        return -1;
    }

    int result = 0;
    unsigned int column = 0;
    for (off_t written = 0; written < TEXT_SIZE;) {
        int character;
        if (column == 1024) {
            character = '\n';
            column = 0;
        } else {
            do {
                character = getc_unlocked(random);
            } while (character != EOF && !isprint(character));
            if (character == EOF) {
                fputs("Unexpected end of random input.\n", stderr);
                result = -1;
                break;
            }
            ++column;
        }
        if (putc_unlocked(character, output) == EOF) {
            perror(path);
            result = -1;
            break;
        }
        ++written;
    }
    fclose(random);
    if (fclose(output) == EOF) {
        perror(path);
        result = -1;
    }
    return result;
}

int main(void)
{
    const char *path = "text.txt";
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0 || (size_t)page_size > SIZE_MAX / 1024) {
        fputs("Could not determine a usable page size.\n", stderr);
        return 1;
    }
    if (generate_text(path) == -1)
        return 1;

    int fd = open(path, O_RDWR);
    if (fd == -1) {
        perror(path);
        return 1;
    }

    size_t chunk_size = (size_t)page_size * 1024;
    unsigned long long uppercase = 0;
    for (off_t offset = 0; offset < TEXT_SIZE;) {
        size_t left = (size_t)(TEXT_SIZE - offset);
        size_t length = left < chunk_size ? left : chunk_size;
        unsigned char *chunk = mmap(NULL, length, PROT_READ | PROT_WRITE,
                                    MAP_SHARED, fd, offset);
        if (chunk == MAP_FAILED) {
            perror("mmap");
            close(fd);
            return 1;
        }
        for (size_t i = 0; i < length; ++i) {
            if (isupper(chunk[i])) {
                ++uppercase;
                chunk[i] = (unsigned char)tolower(chunk[i]);
            }
        }
        int sync_result = msync(chunk, length, MS_SYNC);
        if (sync_result == -1)
            perror("msync");
        if (munmap(chunk, length) == -1) {
            perror("munmap");
            close(fd);
            return 1;
        }
        if (sync_result == -1) {
            close(fd);
            return 1;
        }
        offset += (off_t)length;
    }
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    printf("Capital letters: %llu\n", uppercase);
    return 0;
}
