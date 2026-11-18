# CTF `zhaitie` — report

**Instance:** `findingcheburashka` (`cctf`).

## Reading the library interface

The challenge supplies a compiled decompression library, `test.zai`, and an interface description in `zhaitie-AGENTS.md`. The library returns the size of the next compressed input window. Decompressed bytes arrive through a callback, independently of that return value.

The required sequence is to initialize once, create a context, supply exactly 24 bytes, and then supply exactly the number of bytes requested by each call. A return value of zero means the stream is complete.

## Solution

The decompressor writes callback data immediately and rejects a truncated input window before calling the library:

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct razzhaitie_ctx razzhaitie_ctx;
typedef void (*output_cb)(const unsigned char *, int);
extern void zhaitie_init(void);
extern razzhaitie_ctx *razzhaitie_new(void);
extern int razzhaitie(razzhaitie_ctx *, const unsigned char *, int, output_cb);

static FILE *output;
static int write_failed;

static void write_output(const unsigned char *bytes, int count)
{
    if (count < 0 || (count > 0 &&
        fwrite(bytes, 1, (size_t)count, output) != (size_t)count))
        write_failed = 1;
}

int main(void)
{
    FILE *input = fopen("test.zai", "rb");
    if (!input) { perror("test.zai"); return 1; }
    output = fopen("presentation.bin", "wb");
    if (!output) { perror("presentation.bin"); fclose(input); return 1; }

    zhaitie_init();
    razzhaitie_ctx *ctx = razzhaitie_new();
    int needed = 24;
    int failed = 0;
    while (needed > 0 && !write_failed) {
        unsigned char *buffer = malloc((size_t)needed);
        if (!buffer) { failed = 1; break; }
        if (fread(buffer, 1, (size_t)needed, input) != (size_t)needed) {
            free(buffer);
            failed = 1;
            break;
        }
        needed = razzhaitie(ctx, buffer, needed, write_output);
        free(buffer);
    }
    if (needed < 0 || write_failed) failed = 1;
    if (fclose(input) != 0) failed = 1;
    if (fclose(output) != 0) failed = 1;
    return failed;
}
```

Save this as `decompress.c`. For x86-64 Linux, link the matching archive after the source file:

```bash
gcc -O2 -Wall -Wextra decompress.c libzhaitie-x86_64-linux.a -o decompress
./decompress
strings presentation.bin | grep 'cctf{'
```

For another architecture, use the corresponding library build. The callback pointer belongs to the library, so its bytes are consumed during the callback rather than retained. The context is opaque and has no public destructor; it must not be passed to `free()`.

The recovered presentation contains the flag. The key distinction is between input flow control and output: `razzhaitie()` returns the next input size, while the callback receives the decompressed contents.
