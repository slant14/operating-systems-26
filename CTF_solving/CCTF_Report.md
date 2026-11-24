# CCTF — Report

**Result:** 3 flags recovered out of the 5 challenges in the reference walkthrough (60% of that set).

The challenges tested C programming, pointers, shared libraries, integer widths and bitwise operations. This report records my recovered flags and explains the mechanisms used to solve them.

## 1. Results

| Challenge | Main technique | Recovered flag |
|---|---|---|
| Cavern | Shared library and use-after-free | `cctf{R3memBer_therE_i5_no_spOon_Fre3_from_nasAL_deMons}` |
| Xorcism | Reversing repeating-key XOR | `cctf{Pleas3_Dont_feED_7he_NASAl_DemOn5}` |
| Legacy | Restoring 16-bit integer arithmetic | `cctf{Th3_90od_o1d_DAys_wiTh_GOoD_0lD_c}` |

## 2. Cavern — Shared libraries and use-after-free

### Investigation

The provided `cavern.c` accepts a shared library as its command-line argument. It uses `dlopen()` to load the library, `dlsym()` to locate its `run()` function and then calls that function.

The challenge defines a structure containing two consecutive 64-byte character arrays:

```c
#define BUF_SIZE 64
struct offering {
    char guess[BUF_SIZE];
    char secret[BUF_SIZE];
};
```

`guess()` allocates this structure, copies the input into `guess`, decodes the flag into `secret`, checks the guess and frees the allocation. Crucially, it returns the same pointer after freeing it.

The relevant memory layout in this challenge is:

```text
Offset 0             Offset 64            Offset 128
| guess[64]          | secret[64]         |
```

### My solution

I created `offer.c` containing the following code, recorded in my personal notes:

```c
#include <stdio.h>

extern void* guess(char*);

void run(void)
{
    char* result = guess("inno");
    printf("%s", result + 64);
}
```

I compiled and ran it in the assigned CTF environment:

```bash
gcc -shared -fPIC offer.c -o offer.so
./cavern ./offer.so
```

Initially I used `./cavern offer.so`, which resulted in:

```text
the demons reject your offering: offer.so: cannot open shared object file: No such file or directory
```

The fix was to pass `./offer.so` rather than just `offer.so`. With a slash in the filename, `dlopen()` treats it as a pathname. Without a slash, the dynamic loader searches its configured library locations, which do not normally include the current directory.

### Recorded result

```text
"Wrong," the demons cackle. "Again!"
cctf{R3memBer_therE_i5_no_spOon_Fre3_from_nasAL_deMons}
```

The first line is expected: `"inno"` is not the correct guess. The flag comes from the memory returned by `guess()`, not from successfully guessing the password.

### What happened and why

- `calloc()` allocated zero-initialized memory for the structure. The challenge decoded the secret using a repeating XOR key and stored it in the second array.
- `free()` released the allocation but did not guarantee that the bytes were erased.
- The returned pointer was dangling. In this particular CTF environment, the secret bytes remained accessible long enough to be printed.
- Converting the returned pointer to `char *` makes pointer arithmetic byte-based: `result + 64` corresponds to the start of the second array in this structure.
- Accessing freed memory is **undefined behaviour in C**. The observed success is not a guarantee that another allocator, compiler or operating system will behave the same way.

## 3. Xorcism — Repeating-key XOR

### Investigation

The program uses a 33-byte encrypted array `enc` and a repeating four-byte key. XOR is reversible: applying the same key twice restores the original byte, because `x XOR x = 0` and `x XOR 0 = x`.

### My actual solution (`demon.c`)

```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x67, 0x4d, 0x3f, 0x6e, 0x44, 0x12, 0x05, 0x4b, 0x58, 0x4f, 0x2e, 0x50, 0x51, 0x44, 0x1f, 0x4b, 0x68, 0x16, 0x32, 0x6a, 0x68, 0x6f, 0x1b, 0x5c, 0x76, 0x4d, 0x05, 0x4b, 0x52, 0x4c, 0x15, 0x61, 0x02
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void)
{
    char in[64] = {0};

    fputs("crack me!\n", stdout);
    for (int i = 0; i < PWLEN; i++) {
        in[i] = enc[i] ^ key[i % 4];
    }
    fputs("password: ", stdout);

    if (strlen(in) != PWLEN) {
        puts("nope");
        return 1;
    }

    printf("correct! flag: cctf{%s}\n", in);
    return 0;
}
```

The expression `enc[i] ^ key[i % 4]` decodes one byte at a time. The remainder operator `% 4` cycles through the key indices 0, 1, 2 and 3. The `^` operator performs bitwise XOR, not exponentiation.

The array `in` has 64 zero-initialized bytes. The loop fills its first 33 bytes, leaving a zero terminator at index 33. `strlen(in)` checks whether the reconstructed string has the expected length. The printed password prompt is only output: this version calculates the password directly and does not read input.

### Build and run

```bash
gcc demon.c -o demon
./demon
```

### Verified result

```text
cctf{Pleas3_Dont_feED_7he_NASAl_DemOn5}
```

I independently decoded the 33 encrypted bytes using the four-byte key from my uploaded `demon.c`. The result exactly matches my recorded flag. This is a source-level verification, not a new run in the CTF environment.

### Example of XOR reversal

```text
Plain:       0x41 = 'A'
Key:         0x37
Encrypted:   0x41 XOR 0x37 = 0x76
Decrypted:   0x76 XOR 0x37 = 0x41 = 'A'
```

## 4. Legacy — Integer width and document reconstruction

### Problem

The legacy program was designed around a 16-bit hash accumulator. A normal `unsigned int` on the usual modern Linux ABI is 32 bits, so it does not reproduce the original 16-bit wraparound. This changes the document positions and can cause out-of-bounds writes.

### My actual corrected code (`legacy.c`)

The uploaded source uses `u_int16_t` from `<sys/types.h>`, not the `uint16_t` spelling in the earlier reference:

```c
#include <limits.h>
#include <sys/types.h>
#include <stdio.h>

unsigned position(const char *s)
{
    u_int16_t h = 0;
    while (*s) {
        h = h * 67 + *s++;
    }
    return h;
}
```

`u_int16_t` is an unsigned 16-bit type available on the target system. The standard C alternative is `uint16_t` from `<stdint.h>`. The function still returns `unsigned`, but widening the final result does not restore the bits already discarded during hashing.

The actual recurrence is:

```text
h_next = (67 * h_current + character) mod 65536
```

On the target system, integer promotion can evaluate the expression in a wider type; assigning it to `h` converts the result back to 16 bits. `*s++` means `*(s++)`: read the current character, then advance the pointer.

### How the document is reconstructed

The rest of my actual program contains:

```c
void add_data(char* document, const char* data)
{
    unsigned pos = position(data);
    document[pos] = *data;
}
```

Inside `decrypt_document()`, the buffer is initialized with:

```c
char document[69] = {0};
```

The program calls `add_data()` 33 times with different five-character strings. Each complete string determines an index through `position()`, while only its first character is stored at that index. The calls need not appear in the correct output order.

Examples calculated directly from the uploaded code:

| Input string | 16-bit hash (index) | Stored character |
|---|---:|---|
| `T2WnM` | 0 | `T` |
| `h0QBj` | 1 | `h` |
| `32Gfh` | 2 | `3` |
| `A1EUf` | 14 | `A` |
| `c0ehx` | 32 | `c` |

The 33 indices in this particular source are all distinct and cover exactly 0–32. The remaining bytes of the 69-byte buffer stay zero; index 33 therefore terminates the reconstructed C string. Finally, the program prints it inside `cctf{...}`.

### Build and run

To reproduce the solution from my uploaded source on the Linux CTF machine:

```bash
gcc legacy.c -o legacy
./legacy
```

These commands use the actual uploaded filename; the exact historical output filename was not recorded.

`main()` calls `diagnostics()` first, then `decrypt_document()`. The diagnostics print the width of `u_int16_t` using `sizeof(u_int16_t) * CHAR_BIT` and a hash check value for a fixed test string.

### Verified result

```text
cctf{Th3_90od_o1d_DAys_wiTh_GOoD_0lD_c}
```

## 5. Conclusion

Cavern demonstrates a memory-lifetime error and the distinction between a file path and dynamic-library search. Xorcism demonstrates reversing a known bitwise transformation. Legacy demonstrates how fixed-width integer types preserve the behaviour of an older algorithm. All three recovered flags are recorded above.
