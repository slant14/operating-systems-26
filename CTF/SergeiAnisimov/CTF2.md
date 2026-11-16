# CTF 2 Sergei Anisimov CSE-04

**C language CTF (`cctf`) · Instance `findingcheburashka` · 3 of 5 challenges solved**

The commands below run in Linux or WSL. Use a directory containing the starter files `demon.c`, `legacy.c`, `vaslang.c`, and `inp.vas`. Each section explains the original behavior and shows how to reproduce the result. The programs print the submission strings in `cctf{…}` format.

- [xorcism](#xorcism) · Reverse the byte-wise XOR in `demon.c`.
- [legacy](#legacy) · Reproduce the original machine's 18-bit arithmetic.
- [vaslang](#vaslang) · Resolve forward calls and initialize the value stack.

## xorcism

`demon.c` expects a 33-byte string. For byte `i` the input check computes `enc[i] ^ key[i % 4]` and compares it with `in[i]`. Both arrays are present in the source. The index `i % 4` cycles through the four key bytes.

The first encoded byte is `0x67`. XOR with the first key byte, `0x37`, gives `0x50`, which is ASCII `P`. Applying the same operation to all 33 bytes reconstructs the input. The compiler command builds the supplied program. The Python command reads the two arrays, decodes the bytes, and passes the resulting line to the program through a pipe.

```bash
gcc -std=c11 -Wall -Wextra -Werror demon.c -o demon
python3 - <<'PY' | ./demon
from pathlib import Path
import re

source = Path('demon.c').read_text()

def read_array(name):
    match = re.search(r'\b' + name + r'\s*\[[^\]]+\]\s*=\s*\{([^}]+)\}', source)
    return bytes(int(value, 16) for value in re.findall(r'0x([0-9a-fA-F]+)', match[1]))

encoded = read_array('enc')
key = read_array('key')
assert len(encoded) == 33 and len(key) == 4
decoded = bytes(value ^ key[index % len(key)] for index, value in enumerate(encoded))
print(decoded.decode('ascii'))
PY
```

Successful validation prints `password: correct! flag: cctf{…}`. The password is the decoded text. The submission is the string the program prints with the `cctf{}` wrapper.

## legacy

The original diagnostics describe an 18-bit `unsigned` and give the expected check value `237614`. The function `position()` updates a hash with `h * 67 + byte`. `add_data()` uses that hash as an index into a 69-byte document. On a machine with 32-bit `unsigned`, `position("g1A7g")` gives `2090598423`, so the first write is outside the document. Applying 18-bit wraparound gives index `23` for the same input.

Start with a working copy. Use `<inttypes.h>` for `uint32_t`, `UINT32_C`, and `PRIu32`, keep `<stdio.h>`, and add `<stdlib.h>` for `exit()`. Define the original bit width and document size as follows.

```bash
cp legacy.c legacy-fixed.c
```

```c
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

enum { LEGACY_UNSIGNED_BITS = 18, DOCUMENT_SIZE = 69 };
#define LEGACY_UNSIGNED_MASK ((UINT32_C(1) << LEGACY_UNSIGNED_BITS) - UINT32_C(1))

uint32_t position(const char *s)
{
    uint32_t h = 0;
    while (*s) {
        h = (h * UINT32_C(67) + (unsigned char)*s++) & LEGACY_UNSIGNED_MASK;
    }
    return h;
}
```

The mask is `0x3FFFF`, or `2^18 - 1`. Keeping only those bits after each update implements arithmetic modulo `2^18`. The wider accumulator gives a portable way to perform the computation and apply that mask.

In `diagnostics()`, print `LEGACY_UNSIGNED_BITS` with `%d` and label it as the emulated width. Change the check value's format string to `" check value: %" PRIu32 "\n"` and keep the original `position(...)` argument. In `decrypt_document()`, use `char document[DOCUMENT_SIZE] = {0};` and keep the original `add_data()` calls. Use this checked version of `add_data()`. The last byte of `document` is reserved for the string terminator.

```c
void add_data(char *document, const char *data)
{
    uint32_t pos = position(data);
    if (pos >= DOCUMENT_SIZE - 1) {
        fprintf(stderr, "Document position out of range: %" PRIu32 "\n", pos);
        exit(EXIT_FAILURE);
    }
    document[pos] = *data;
}
```

```bash
gcc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined,bounds \
    -fno-sanitize-recover=all legacy-fixed.c -o legacy
./legacy
```

The corrected program printed `emulated unsigned has 18 bits`, `check value: 237614`, and one `cctf{…}` line under `Decrypted Document`. The diagnostic confirms the intended arithmetic before checking the decrypted output.

## vaslang

The interpreter reads definitions of the form `name: body` and runs a function when it sees `name!`. Numbers push bytes onto the value stack, `+` and `*` combine the top two values modulo 251, `:` duplicates the top value, and `.` drops it. The built-in `hello!` command prints `hello` and gives a small working example of those operations.

To reproduce the original failure, build the starter interpreter with `_GNU_SOURCE`, which declares the library functions it uses. Redirect its large stream of prompts and messages to a file.

```bash
gcc -D_GNU_SOURCE -Wall -Wextra vaslang.c -o vaslang-original
./vaslang-original < inp.vas > vaslang-original.out
grep -m 5 'ERROR:' vaslang-original.out
tail -n 1 inp.vas
```

The first error was `ERROR: func nervous_168 undefined`. The input ends with `main!` and contains 2,106 definitions, with 3,028 calls naming functions whose definitions appear later. The original compiler required the callee's body immediately and stopped compiling a definition on the first unresolved call.

The fix stores a function entry as soon as its name is encountered. If the body is already known, the compiled call uses that body. Otherwise, it keeps a reference to the function entry. Defining the function fills in the entry, and the interpreter looks up its body when executing the deferred call. The explicit return stack records the instruction after each call so execution can resume there.

After resolving calls in an independent simulation, one of the first operations reached by `main` was `:` on an empty stack. The input is consistent with a zero in the initial slot. With that slot in place, `:` can duplicate the zero and the remaining byte operations complete. Returning from the top-level function prints the bytes after the reserved slot and stops at the first zero or the end of the stack. The interpreter also checks stack bounds and reports undefined functions instead of dereferencing a missing body.

Save the complete corrected source below as `vaslang-fixed.c` next to `inp.vas`. Compile and run it with the supplied input. The pipe keeps the flag line readable among the interpreter prompts.

```bash
gcc -std=c11 -O2 -Wall -Wextra -Werror -pedantic -fsanitize=undefined \
    -fno-sanitize-recover=all vaslang-fixed.c -o vaslang
./vaslang < inp.vas | grep -o 'cctf{[^}]*}'
```

The full input printed `hello` at startup and exactly one `cctf{…}` result. A shorter test defines `first` before `later` and then invokes `first!`. It should print `A` because `later` pushes `65` (ASCII `A`) followed by zero.

```bash
printf 'first: later\nlater: 65 0\nfirst!\n' | ./vaslang
```

<details>
<summary>Complete corrected vaslang-fixed.c</summary>

```c
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    TERM_CAPACITY = 1048576,
    FUNC_CAPACITY = 65536,
    STACK_CAPACITY = 512,
    LINE_CAPACITY = 640 * 1024
};

struct func;

struct term {
    char ty;
    union {
        struct term *target;
        struct func *callee;
        unsigned char val;
    };
};

struct func {
    char *name;
    struct term *body;
};

static struct term terms[TERM_CAPACITY];
static struct func funcs[FUNC_CAPACITY];
static struct term *termp = terms;
static struct func *funcp = funcs;

static void fail(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    fputs("ERROR: ", stderr);
    vfprintf(stderr, format, args);
    fputc('\n', stderr);
    va_end(args);
    exit(EXIT_FAILURE);
}

static struct func *find_func(const char *name)
{
    for (struct func *func = funcs; func < funcp; ++func) {
        if (strcmp(func->name, name) == 0)
            return func;
    }
    return NULL;
}

static struct func *intern_func(const char *name)
{
    struct func *func = find_func(name);
    if (func)
        return func;
    if (funcp == funcs + FUNC_CAPACITY)
        fail("too many functions");

    size_t name_size = strlen(name) + 1;
    char *copy = malloc(name_size);
    if (!copy)
        fail("out of memory");
    memcpy(copy, name, name_size);

    funcp->name = copy;
    return funcp++;
}

static struct term *new_term(void)
{
    if (termp == terms + TERM_CAPACITY)
        fail("too many instructions");
    return termp++;
}

static void run(const struct term *term)
{
    const struct term *returns[STACK_CAPACITY];
    const struct term **return_top = returns;
    unsigned char values[STACK_CAPACITY + 1] = {0};
    unsigned char *value_top = values + 1;
    const unsigned char *const value_end = values + sizeof values;

    for (;;) {
        switch (term->ty) {
        case '+':
        case '*': {
            if (value_top - values < 2)
                fail("too few operands for %c", term->ty);
            unsigned right = *--value_top;
            unsigned left = value_top[-1];
            unsigned result = term->ty == '+' ? left + right : left * right;
            value_top[-1] = (unsigned char)(result % 251);
            break;
        }
        case ':':
            if (value_top == values)
                fail("nothing to duplicate");
            if (value_top == value_end)
                fail("value stack full");
            *value_top = value_top[-1];
            ++value_top;
            break;
        case '.':
            if (value_top == values)
                fail("nothing to drop");
            --value_top;
            break;
        case 'P':
            if (value_top == value_end)
                fail("value stack full");
            *value_top++ = term->val;
            break;
        case 'C':
        case 'F': {
            const struct term *target =
                term->ty == 'C' ? term->target : term->callee->body;
            if (!target)
                fail("function %s undefined", term->callee->name);
            if (return_top == returns + STACK_CAPACITY)
                fail("call stack full");
            *return_top++ = term + 1;
            term = target;
            continue;
        }
        case 'R':
            if (return_top == returns) {
                /* The first slot is the initial zero used by the program. */
                for (const unsigned char *value = values + 1;
                     value < value_top && *value; ++value)
                    putchar(*value);
                putchar('\n');
                return;
            }
            term = *--return_top;
            continue;
        default:
            fail("invalid instruction");
        }
        ++term;
    }
}

static void xcmd(const char *name)
{
    const struct func *func = find_func(name);
    if (!func || !func->body)
        fail("function %s undefined", name);
    run(func->body);
}

static void xdef(const char *name, char *body)
{
    struct func *func = intern_func(name);
    func->body = termp;

    for (char *token = strtok(body, " \t\r\n"); token;
         token = strtok(NULL, " \t\r\n")) {
        struct term *term = new_term();
        if (token[1] == '\0' && strchr("+*:.", token[0])) {
            term->ty = token[0];
            continue;
        }

        char *end;
        long value = strtol(token, &end, 10);
        if (end != token && *end == '\0') {
            term->ty = 'P';
            term->val = (unsigned char)value;
            continue;
        }

        struct func *callee = intern_func(token);
        if (callee->body) {
            term->ty = 'C';
            term->target = callee->body;
        } else {
            term->ty = 'F';
            term->callee = callee;
        }
    }
    new_term()->ty = 'R';
}

static void xline(char *line)
{
    char *separator = strchr(line, '!');
    if (separator) {
        *separator = '\0';
        xcmd(line);
        return;
    }

    separator = strchr(line, ':');
    if (!separator)
        fail("expected a command or definition");
    *separator = '\0';
    xdef(line, separator + 1);
}

int main(void)
{
    char line[LINE_CAPACITY];
    char greeting[] = "hello: 104 : 248 + : 180 * : 111 0";
    xline(greeting);
    xcmd("hello");

    for (;;) {
        fputs("> ", stdout);
        if (!fgets(line, sizeof line, stdin))
            break;
        line[strcspn(line, "\r\n")] = '\0';
        xline(line);
    }

    for (struct func *func = funcs; func < funcp; ++func)
        free(func->name);
    return ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS;
}
```

</details>
