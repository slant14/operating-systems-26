# legacy

Flag: cctf{THe_g0oD_o1d_Days_WIth_good_0lD_C}

Finding: the program was written for an old machine where unsigned is 18 bits.
On our 32-bit machine its hash wraps at the wrong point, so it writes out of
bounds and segfaults. I made the hash wrap at 18 bits again (one line).

Locations:
- No ssh container for this one: the bug is in the code, so it is solved locally from the given source (legacy.c), on my own machine.
- Bug: function position() in legacy.c, the rolling hash: while (*s) h = h*67 + *s++;
  It expects to wrap at 2^18 (262144).
- Out-of-bounds write: add_data() does document[pos] = *data; into char document[69],
  with pos = position(data). On 32 bits pos is huge -> crash.
- Data: 33 fixed 5-char strings in decrypt_document(). Each one hashes (mod 2^18) to a
  slot 0..32, and its first char is the byte that goes there.
- Fix: legacy_fixed.c, in position(): h = (h * 67 + *s++) & 0x3FFFFu;  (0x3FFFF = 2^18-1).
  After this the diagnostic check value is 237614, same as the original machine.

Commands:
  gcc -O0 -o legacy_fixed legacy_fixed.c
  ./legacy_fixed

Output:
   check value: 237614
  == Decrypted Document: ==
  cctf{THe_g0oD_o1d_Days_WIth_good_0lD_C}
