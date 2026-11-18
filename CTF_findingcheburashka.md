# CTF findingcheburashka · CSE

**C language CTF (`cctf`) · Instance `findingcheburashka` · 3 of 5 challenges solved**

The commands below are run on Linux or WSL. You'll need the files `demon.c`, `legacy.c`, `vaslang.c`, and `inp.vas`. Each section briefly explains what was wrong and how it was fixed. The programs print flags in the format `cctf{…}`.

- [xorcism](#xorcism) · Reverse the XOR in `demon.c`.
- [legacy](#legacy) · Reproduce 18-bit arithmetic like on the old machine.
- [vaslang](#vaslang) · Fix forward calls and the stack.

## xorcism

`demon.c` checks a 33-byte string. For each byte it computes `enc[i] ^ key[i % 4]` and compares it against the input. The `enc` and `key` arrays are already in the code; the 4-byte key repeats.

The first byte `0x67` XORed with `0x37` gives `0x50` (`P`). Doing the same for the rest of the bytes recovers the password. Built the program and fed it the decrypted string:

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
decoded = bytes(value ^ key[index % len(key)] for index, value in enumerate(encoded))
print(decoded.decode('ascii'))
PY
```

If everything's right, the program prints `correct! flag: cctf{…}`. The flag is whatever it printed.

## legacy

The diagnostics say `unsigned` used to be 18-bit, and the check value should be `237614`. The `position()` function computes a hash as `h * 67 + byte`, and `add_data()` uses it as an index into a 69-byte document. On a regular machine with a 32-bit `unsigned`, the index runs out of bounds and the program crashes.

Made a copy and added an 18-bit mask:

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

The `0x3FFFF` mask truncates the hash to 18 bits after every step. In `diagnostics()`, changed the output to the emulated width and check-value format. In `add_data()`, added a bounds check to avoid writing out of range (just in case, even though the mask should prevent it).

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
gcc -std=c11 -Wall -Wextra -Werror legacy-fixed.c -o legacy
./legacy
```

After that, the program showed 18 bits, a check value of `237614`, and the flag in `Decrypted Document`. At first I forgot to change the check-value output format and couldn't figure out for a while why the number didn't match, then noticed it.

## vaslang

The interpreter reads `name: body` definitions and runs a function via `name!`. Numbers push a byte onto the stack, `+` and `*` add/multiply modulo 251, `:` duplicates, `.` discards. There's a `hello!` example that prints `hello`.

The original crashed immediately. Built it and ran the input:

```bash
gcc -D_GNU_SOURCE -Wall -Wextra vaslang.c -o vaslang-original
./vaslang-original < inp.vas > vaslang-original.out
grep -m 5 'ERROR:' vaslang-original.out
```

The first error was `ERROR: func nervous_168 undefined`. The input has many calls to functions defined later on. The old code required the body to already be known at the time the call was compiled, and it would stop.

Fixed it like this: as soon as a name is encountered, a function entry is created right away. If the body is already known, the call is compiled immediately. If not, a reference to the entry is stored, and the body is patched in later once the function is defined. Added a return stack so execution can come back after a call.

Another problem: right at the start, `main` did a `:` on an empty stack. Turned out a zero needs to be placed in the initial stack cell beforehand. Once that was done, everything else proceeded. When the top-level function returns, the bytes on the stack up to the first zero are printed.

Saved the full fixed code as `vaslang-fixed.c`. Build and run:

```bash
gcc -std=c11 -O2 -Wall -Wextra vaslang-fixed.c -o vaslang
./vaslang < inp.vas | grep -o 'cctf{[^}]*}'
```

On the full input it printed `hello` and one flag. A short test also works:

```bash
printf 'first: later\nlater: 65 0\nfirst!\n' | ./vaslang
```

Should print `A`. Spent a bit of time debugging the stack, didn't notice at first that the initial zero was needed.
