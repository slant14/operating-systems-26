# CTF `legacy` — report

**Instance:** `findingcheburashka` (`cctf`).

## Finding the mismatch

The supplied program calculates a position with a rolling hash and uses it to write a character into `document`. Its original developer's diagnostics give two important values:

```text
unsigned has 18 bits
check value: 237614
```

On a modern system, `unsigned` normally has 32 bits. The expression `h * 67 + byte` therefore wraps at a different width and produces different array positions. This explains the crash without requiring a change to the encoded strings.

## Solution

Keep the low 18 bits after every hash step:

```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h * 67u + (unsigned char)*s++) & 0x3FFFFu;
    }
    return h;
}
```

```bash
cp legacy.c legacy-fixed.c
# Replace position() in legacy-fixed.c with the version above.
gcc -Wall -Wextra legacy-fixed.c -o legacy-fixed
./legacy-fixed
```

The mask is `2^18 - 1`, so it reproduces the arithmetic of the original machine. A 16-bit type would discard two additional bits and would not implement the required behavior.

For the supplied personal source, the 33 encoded strings map to distinct positions 0 through 32 with this calculation. The diagnostic hash becomes `237614`. The program then assembles the characters and prints the flag under `Decrypted Document`. The diagnostic line reporting the native size of `unsigned` may still say 32 bits; the mask emulates the required arithmetic without changing the C type.
