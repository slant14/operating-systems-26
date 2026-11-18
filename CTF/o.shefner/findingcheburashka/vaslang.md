# CTF `vaslang` — report

**Instance:** `findingcheburashka` (`cctf`).

## Inspecting the interpreter

The challenge provides a small stack-language interpreter, `vaslang.c`, and a large program, `inp.vas`. Definitions use `name: body`; a command such as `main!` executes a named function. Numbers push values, `+` and `*` calculate modulo 251, `:` duplicates the top value, and `.` drops it.

The supplied input has 1956 definitions and ends with `main!`. There are 2520 calls to names that have not yet appeared in definition order, although every called function is defined somewhere in the file.

The original `xdef()` resolves calls immediately. A forward reference therefore produces an undefined-function error and returns from parsing before the definition is complete. Later functions inherit the resulting broken state.

## Solution

Separate storing definitions from resolving calls. Each function name needs a stable entry in the function table, even before its body is parsed:

```c
struct term {
    char ty;
    union {
        struct func *target;
        unsigned char val;
    };
};
```

When a name first appears, create its function entry with an empty body. A later definition fills that same entry. Compile a call by storing a pointer to the entry rather than a copy of its current body pointer. At execution time, the call follows `target->body`.

```c
case 'C':
    *fsp++ = t;
    t = t->target->body - 1;
    break;
```

The `-1` compensates for the interpreter's `t++` after the switch. Returning restores the saved call instruction, and the same increment advances to the next instruction. Only a genuinely new function should increment `funcp`; redefining an existing name must update its existing entry.

Before execution, check that every referenced entry has a body. The value and return stacks also need bounds checks so that malformed input reports an error instead of accessing memory outside the arrays.

```bash
gcc -D_GNU_SOURCE -O2 -Wall -Wextra vaslang-fixed.c -o vaslang-fixed
./vaslang-fixed < inp.vas > output.txt
grep -o 'cctf{[^}]*}' output.txt
```

A small forward-reference example isolates the important behavior:

```text
first: later
later: 65 0
first!
```

It should print `A`. Once references are resolved against the completed function table, the final `main!` can execute the supplied program and produce the flag. This is a name-resolution problem: changing the order of unrelated arithmetic operations would change the program rather than repair the interpreter.
