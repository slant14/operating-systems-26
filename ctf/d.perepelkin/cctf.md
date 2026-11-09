### 1. cavern (offer.c)
* Calling an external library function `guess()` and reading memory at offset `+64` bytes past the returned pointer.
* **Commands & Code (`offer.c`):**
```c
#include <stdio.h>

extern void* guess(char*);

void run(void)
{
    char* result = guess("test");
    printf("%s", result + 64);
}
```

```bash
gcc -shared -fPIC offer.c -o offer.so
# (run challenge binary with compiled shared object to print flag at result + 64)
```

### 2. legacy (new.c)

- The legacy code assumes unsigned is 16-bit (uint16_t, max 0xFFFF), whereas on modern 64-bit Linux (LP64) unsigned is 32-bit, causing position() (& 0x3FFFF) to index out of bounds of char document. Masking hash h with 0xFFFF (16-bit overflow) restores the intended indices 0..32.
```C
#include <stdint.h>
#include <stdio.h>

unsigned position(const char* s)
{
    uint16_t h = 0; // Fixed to 16-bit unsigned integer
    while (*s)
    {
        h = (h * 67 + *s++);
    }
    return h;
}
```

```bash
gcc new.c -o new && ./new
```
### 3. xorcism (demon.c)

- Reverse-engineering the static XOR check in demon: XORing the 33-byte enc array with the 4-byte repeating key = { 0x37, 0x21, 0x5a, 0x0f }.
```C
#include <stdio.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x4d, 0x69, 0x6e, 0x44, 0x44, 0x05, 0x4b, 0x78, 0x4f, 0x2e,
    0x50, 0x51, 0x64, 0x3f, 0x6b, 0x68, 0x16, 0x32, 0x6a, 0x68, 0x4f,
    0x6e, 0x7c, 0x56, 0x6d, 0x05, 0x6b, 0x72, 0x6c, 0x35, 0x41, 0x44
};
static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    char in[64] = { 0 };
    for (int i = 0; i < PWLEN; ++i)
        in[i] = enc[i] ^ key[i % 4];

    printf("correct! flag: cctf{%s}\n", in);
    return 0;
}
```

```bash
gcc demon.c -o demon && ./demon
```

### 4. zhaitie (zhatie.c)

- Interfacing with the custom decompression library (zhaitie_init, razzhaitie_new, razzhaitie), dynamically resizing the read buffer to the chunk size requested by razzhaitie(), and writing decompressed chunks via callback to presentation_unpacked.bin.    
```C
#include <stdio.h>
#include <stdlib.h>

struct razzhaitie_ctx;
typedef void (*razzhaitie_output_cb)(const unsigned char*, int);

extern const char* zhaitie_version();
extern void zhaitie_init();
extern struct razzhaitie_ctx* razzhaitie_new();
extern int razzhaitie(struct razzhaitie_ctx* ctx, const unsigned char* bytes, int bytesCount, razzhaitie_output_cb outputCallback);

static FILE* outFile = NULL;

void callback(const unsigned char* chunk, const int length)
{
    if (!outFile || length <= 0) return;
    fwrite(chunk, sizeof(char), (size_t)length, outFile);
}

int main(void)
{
    zhaitie_init();
    struct razzhaitie_ctx* ctx = razzhaitie_new();
    FILE* file = fopen("test.zai", "rb");
    outFile = fopen("presentation_unpacked.bin", "wb");

    int bufferSize = 24;
    unsigned char* buffer = malloc((size_t)bufferSize);
    fread(buffer, 1, (size_t)bufferSize, file);
    bufferSize = razzhaitie(ctx, buffer, bufferSize, callback);

    while (bufferSize)
    {
        buffer = realloc(buffer, (size_t)bufferSize);
        fread(buffer, 1, (size_t)bufferSize, file);
        bufferSize = razzhaitie(ctx, buffer, bufferSize, callback);
    }

    free(buffer);
    fclose(outFile);
    fclose(file);
    return 0;
}
```

```bash
gcc zhatie.c -L. -lzhaitie -Wl,-rpath,. -o zhatie && ./zhatie strings presentation_unpacked.bin | grep "cctf"
```

### 5. vaslang (vaslang.c)

- Stack-based bytecode interpreter. Fixed interpreter source vaslang.c to evaluate the ROM/flag routine.
```bash
gcc vaslang.c -o vaslang && ./vaslang
```

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errorf(fmt, ...) {\
    printf("ERROR: " fmt "\n", ##__VA_ARGS__);\
    return;}

struct term
{
    char ty;

    union
    {
        struct func* target;
        unsigned char val;
    };
};

struct func
{
    char* name;
    struct term* body;
};

struct term terms[1048576] = { 0 };
struct func funcs[65536] = { 0 };
struct term* termp = terms;
struct func* funcp = funcs;

struct term* fs[65536] = { 0 };
unsigned char vs[65536] = { 0 };
struct term** fsp = fs;
unsigned char* vsp = vs;

void run(struct term* t)
{
    unsigned l, r;
    fsp = fs;
    vsp = vs;

    while (1)
    {
        switch (t->ty)
        {
        case '+':
            l = *--vsp;
            r = *--vsp;
            *vsp++ = (l + r) % 251;
            break;
        case '*':
            l = *--vsp;
            r = *--vsp;
            *vsp++ = (l * r) % 251;
            break;
        case ':':
            l = *--vsp;
            *vsp++ = l;
            *vsp++ = l;
            break;
        case '.':
            vsp--;
            break;
        case 'P':
            *vsp++ = t->val;
            break;
        case 'C':
            *fsp++ = t;
            t = t->target->body - 1;
            break;
        case 'R':
            if (fsp == fs)
            {
                for (vsp = vs; *vsp; putchar(*vsp++));
                putchar('\n');
                return;
            }

            t = *--fsp;
            break;
        }
        t++;
    }
}

void xcmd(char* s)
{
    char* name = strsep(&s, "!");
    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            return run(funcs[i].body);
    errorf("func %s undefined", name);
}

struct func* get_func(const char* name)
{
    for (int i = 0; funcs + i < funcp; ++i)
    {
        if (strcmp(funcs[i].name, name) == 0) return funcs + i;
    }
    struct func* f = funcp++;
    f->name = strdup(name);
    f->body = NULL;
    return f;
}

void xdef(const char* sp)
{
    char* s = strdup(sp);
    char* name = strsep(&s, ":");

    struct func* func = get_func(name);
    struct term* ftermp = termp;
    func->body = ftermp;

    while (s != NULL)
    {
        char* term = strsep(&s, " ");
        if (*term == 0)
            continue;

        unsigned char val;
        char* endp;

        if (term[0] == '+' || term[0] == '*' ||
            term[0] == ':' || term[0] == '.')
        {
            ftermp->ty = term[0];
        }
        else if (val = strtol(term, &endp,
                              10), endp != term)
        {
            ftermp->ty = 'P';
            ftermp->val = val;
        }
        else
        {
            ftermp->ty = 'C';
            ftermp->target = get_func(term);
            if (ftermp->target == NULL)
                errorf("func %s undefined", term);
        }
        ftermp++;
    }
    ftermp->ty = 'R';
    ftermp++;

    termp = ftermp;
}

void xline(char* s)
{
    if (strchr(s, '!') != NULL)
        xcmd(s);
    else if (strchr(s, ':') != NULL)
        xdef(s);
    else
        errorf("Failed to parse %s: [command!] or"
           " [definition: body] expected", s)
}

void xrom(const char* s)
{
    char buf[640 * 1024];
    strcpy(buf, s);
    xline(buf);
}

int main()
{
    char buf[640 * 1024];
    xrom("hello: 104 : 248 + : 180 * : 111 0");
    xrom("hello!");
    while (1)
    {
        printf("> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            return 0;
        *strchrnul(buf, '\n') = 0;
        xline(buf);
    }
}

```