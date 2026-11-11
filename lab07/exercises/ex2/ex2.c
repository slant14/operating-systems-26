#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_SIZE (500L * 1024 * 1024) // 500 MiB
#define LINE_LEN 1024

int main()
{
    FILE *frandom = fopen("/dev/random", "r");
    if (frandom == NULL) return 1;

    FILE *text = fopen("text.txt", "w");
    if (text == NULL) {
        fclose(frandom);
        return 1;
    }

    long written = 0;
    int line = 0;
    while (written < FILE_SIZE) {
        // new line after every 1024 characters
        if (line == LINE_LEN) {
            fputc('\n', text);
            written++;
            line = 0;
            continue;
        }

        int c = fgetc(frandom);
        if (c == EOF) break;
        if (!isprint(c)) continue;

        fputc(c, text);
        written++;
        line++;
    }
    fclose(frandom);
    fclose(text);

    // open the file in chunks with mmap
    int fd = open("text.txt", O_RDWR);
    if (fd == -1) return 1;

    struct stat st;
    fstat(fd, &st);
    long file_size = st.st_size;

    long sz = sysconf(_SC_PAGESIZE);
    long chunk_size = 1024 * sz;

    long capitals = 0;
    for (long offset = 0; offset < file_size; offset += chunk_size) {
        long len = file_size - offset < chunk_size ? file_size - offset : chunk_size;

        // MAP_SHARED, so changes are written back to the file
        char *chunk = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
        if (chunk == MAP_FAILED) {
            close(fd);
            return 1;
        }

        for (long i = 0; i < len; i++) {
            if (isupper(chunk[i])) {
                capitals++;
                chunk[i] = tolower(chunk[i]);
            }
        }

        munmap(chunk, len);
    }
    close(fd);

    printf("total number of capital letters: %ld\n", capitals);
}

// total number of capital letters: 143350979
// (text.txt is 524288000 bytes, after the program there are no capital letters in it)
