# cctf — vaslang (100 points)

**Flag:** `cctf{YOU_c0Uld_havE_s0LveD_tHIS_the_FUN_WAy}`

## Task

The bot gives `vaslang.c` (an interpreter of a small stack language, FORTH-like,
all arithmetic modulo 251) and `inp.vas` — a 2000-line program written by an AI.
The AI hit its context limit, so "the final edits came out mangled": the program
crashes the interpreter. Fix the interpreter so the program runs and prints the
flag.

Language ops: `+` add, `*` mul, `:` dup, `.` drop, `<num>` push,
`<func>` call, `R` return (when the return stack is empty, print the value stack
from its start as a string — that is how the flag is printed).

## Bug 1: forward references

`inp.vas` calls functions that are **defined later** in the file. The parser
resolves a call `name` only against already-defined functions:

```c
} else {
    ftermp->ty = 'C';
    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, term) == 0)
            ftermp->target = funcs[i].body;
    if (ftermp->target == NULL)
        errorf("func %s undefined", term);   /* aborts the definition */
}
```

Running the original interpreter floods with `ERROR: func affectionate_300 undefined` etc.

**Fix:** store the name for unresolved calls and resolve it lazily on the first
execution. The name and the target share a union, so we distinguish a resolved
call (target points inside `terms[]`) from a pending one (target holds the
strdup'ed name on the heap):

```c
case 'C':
    if (t->target == NULL ||
        t->target < terms || t->target >= terms + 1048576) {
        const char* nm = (t->target == NULL) ? NULL : t->cname;
        int found = 0;
        if (nm != NULL) {
            for (int i = 0; funcs + i < funcp; i++)
                if (strcmp(funcs[i].name, nm) == 0) {
                    t->target = funcs[i].body;
                    found = 1;
                    break;
                }
        }
        if (!found) { printf("ERROR: func %s undefined\n", nm ? nm : "?"); return; }
    }
    *fsp++ = t;
    t = t->target - 1;
    break;
```

with an extra `char *cname;` added to the `struct term` union, and in the parser:

```c
if (ftermp->target == NULL)
    ftermp->cname = strdup(term);   /* forward reference, resolve at first call */
```

## Bug 2: stacks are too small

After the first fix the program segfaulted in `switch (t->ty)` with a wild
pointer. `gdb` showed the crash inside `run()`. The interpreter has fixed
stacks `fs[512]` / `vs[512]`; the 2000-function program with hundreds of pushes
per body overflows them.

**Fix:**

```c
struct term *fs[65536] = {0};       /* was 512 */
unsigned char vs[1048576] = {0};    /* was 512 */
```

(Minor build fix too: `strchrnul` needs `-D_GNU_SOURCE`.)

## Run

```
gcc -D_GNU_SOURCE -o vaslang_fixed vaslang_fixed.c
./vaslang_fixed < inp.vas
```

Output (after the `hello` self-test):

```
cctf{YOU_c0Uld_havE_s0LveD_tHIS_the_FUN_WAy}
```

## Result

```
✅ Flag accepted for 'vaslang'.   100 points
```
