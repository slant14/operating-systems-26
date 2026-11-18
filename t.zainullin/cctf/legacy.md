# cctf - legacy

**Flag:** `cctf{7He_GooD_0ld_D4y5_Wi7H_gOOD_0Ld_C}`

## Task

The bot gives `legacy.c` - a 42-year-old program. On a modern machine it just
segfaults. The developer's diagnostics say:

```
unsigned has 18 bits
check value: 237614
```

The program computes a hash `position(s)` and writes the **first character** of
each string into a `char document[69]` at index `pos = position(data)`:

```c
unsigned position(const char *s) { unsigned h = 0; while (*s) h = h*67 + *s++; return h; }
void add_data(char* document, const char* data) { unsigned pos = position(data); document[pos] = *data; }
```

Then it prints `cctf{document}`.

## Idea

The old machine had **18-bit unsigned** integers, so the hash was effectively
modulo 2^18 = 262144. On a modern 32-bit `unsigned` the hash values are huge and
`document[pos]` runs out of the 69-byte buffer → segfault.

Check: `position(EICAR-string) mod 2^18 = 237614` - exactly the value from the
diagnostics, so the hypothesis is confirmed.

The "fix" is to reproduce the old behaviour: mask the hash to 18 bits.



Patch only the hash line ( emulation of 18-bit unsigned):

```
sed 's/h = h \* 67 + \*s++;/h = (h * 67 + (unsigned char)*s++) \& 0x3FFFFu;/' legacy.c > legacy18.c
gcc -o legacy18 legacy18.c
./legacy18
```

Output:

```
== Diagnostics: ==
 unsigned has 32 bits
 check value: 237614
== End of Diagnostics ==

Attempting to decrypt the document...
== Decrypted Document: ==
cctf{7He_GooD_0ld_D4y5_Wi7H_gOOD_0Ld_C}
== End of Decrypted Document ==
```

("The Good Old Days With GOOD Old C".) All 33 strings landed at positions 0..32,
the rest of `document` stays zero, so `printf("%s")` stops at the first NUL byte.
