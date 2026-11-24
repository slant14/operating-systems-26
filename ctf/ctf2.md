# Cavern

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <link.h>
#include <stdint.h>

static const unsigned char key[4] = {0xde, 0xad, 0xbe, 0xef};

static int scan_cb(struct dl_phdr_info *info, size_t size, void *data) {
    (void)size; (void)data;
    if (info->dlpi_name && info->dlpi_name[0] != '\0') return 0;

    for (int i = 0; i < info->dlpi_phnum; i++) {
        const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD) continue;
        if (!(ph->p_flags & PF_R)) continue;

        uintptr_t start = info->dlpi_addr + ph->p_vaddr;
        uintptr_t end   = start + ph->p_memsz;

        for (uintptr_t p = start; p + 4 <= end; p++) {
            if (memcmp((void*)p, key, 4) != 0) continue;

            long offsets[2] = { -55, 4 };
            for (int k = 0; k < 2; k++) {
                intptr_t q = (intptr_t)p + offsets[k];
                if (q < (intptr_t)start || q + 55 > (intptr_t)end) continue;

                unsigned char *fenc = (unsigned char*)q;
                char flag[56];
                for (int j = 0; j < 55; j++) flag[j] = fenc[j] ^ key[j % 4];
                flag[55] = 0;

                int printable = 1;
                for (int j = 0; j < 55; j++) {
                    if (flag[j] < 32 || flag[j] > 126) { printable = 0; break; }
                }
                if (printable) printf("%s\n", flag);
            }
        }
    }
    return 0;
}

void run(void) {
    void *h = dlopen(NULL, RTLD_NOW);
    if (h) {
        void *(*guess_fn)(char*) = dlsym(h, "guess");
        if (guess_fn) {
            char dummy[64] = {0};
            void *off = guess_fn(dummy);
            if (off) {
                char *secret = (char*)off + 64;
                int printable = 1;
                for (int i = 0; i < 55; i++) {
                    if (secret[i] < 32 || secret[i] > 126) { printable = 0; break; }
                }
                if (printable) {
                    printf("%.55s\n", secret);
                    return;
                }
            }
        }
    }

    dl_iterate_phdr(scan_cb, NULL);
}
```





# Legacy
``` bash
cat << 'EOF' > legacy_solve.c
```
```c
#include <stdio.h>
#include <string.h>

unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h * 67 + *s++) & 0x3FFFF;
    }
    return h;
}

void add_data(char* document, const char* data)
{
    unsigned pos = position(data);
    document[pos] = *data;
}

int main() {
    char document[69] = {0};

    add_data(document, "H0HaR");
    add_data(document, "32Gfh");
    add_data(document, "D0djD");
    add_data(document, "w0MrP");
    add_data(document, "o0Ki8");
    add_data(document, "o0KiL");
    add_data(document, "C2KyY");
    add_data(document, "_0GVh");
    add_data(document, "g1A7g");
    add_data(document, "_0GVl");
    add_data(document, "_0GW8");
    add_data(document, "O0CDf");
    add_data(document, "l0oTk");
    add_data(document, "i0Xhz");
    add_data(document, "t07CD");
    add_data(document, "_0GVv");
    add_data(document, "h0QBj");
    add_data(document, "D0djH");
    add_data(document, "S0aVR");
    add_data(document, "L0fsR");
    add_data(document, "_0GVc");
    add_data(document, "g1A7T");
    add_data(document, "O0CDb");
    add_data(document, "_1yII");
    add_data(document, "d0mKc");
    add_data(document, "D0dit");
    add_data(document, "T0i9T");
    add_data(document, "a1MzO");
    add_data(document, "d02t9");
    add_data(document, "y1TRq");
    add_data(document, "00BEe");
    add_data(document, "00BEh");
    add_data(document, "_0GVq");

    printf("cctf{%s}\n", document);
    return 0;
}
```
```bash
EOF
gcc legacy_solve.c -o legacy_solve
./legacy_solve
```






# Xorcism
```bash
cat << 'EOF' > solve.c
```
```c
#include <stdio.h>

int main() {
    unsigned char enc[] = {
        0x47, 0x4d, 0x3f, 0x4e, 0x02, 0x12, 0x05, 0x4b, 0x07, 0x6f, 0x2e, 0x50, 0x51, 0x64, 0x1f, 0x4b, 0x68, 0x75, 0x12, 0x6a, 0x68, 0x6f, 0x1b, 0x7c, 0x76, 0x4d, 0x05, 0x4b, 0x52, 0x6c, 0x35, 0x41, 0x02
    };
    unsigned char key[] = { 0x37, 0x21, 0x5a, 0x0f };
    int len = sizeof(enc) / sizeof(enc[0]);

    printf("cctf{");
    for (int i = 0; i < len; i++) {
        putchar(enc[i] ^ key[i % 4]);
    }
    printf("}\n");

    return 0;
}
```
```bash
EOF
gcc solve.c -o solve
./solve
```