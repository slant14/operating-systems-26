# C CTF: 3 accepted challenges

**Instance:** `findingcheburashka` (3/5 accepted).

Send `/challenge NAME` to InnoFlagranteBot for each task. Work in Ubuntu unless an SSH connection is shown. Save in Nano with **Ctrl+O**, **Enter**, **Ctrl+X**. Send each printed `cctf{...}` flag to the bot.

## 1. legacy

The old program uses an **18-bit unsigned integer**. On the current machine the hash does not wrap at the same value.

1. Save the C code supplied by `/challenge legacy`:
   ```bash
   nano legacy.c
   ```
2. In `position()`, replace:
   ```c
   h = h * 67 + *s++;
   ```
   with:
   ```c
   h = (h * 67 + *s++) % 262144;
   ```
3. Compile and run:
   ```bash
   gcc legacy.c -o legacy
   ./legacy
   ```
4. Submit the printed flag. The check value in my run was `237614`.

**Why:** `262144 = 2^18`. The modulo makes the hash wrap like the old 18-bit integer.

**Accepted flag:** `cctf{tHE_g00d_0ld_daYS_WiTH_goOd_Old_C}`

## 2. xorcism

The program stores its password as bytes encoded with XOR.

1. Save the code supplied by `/challenge xorcism`:
   ```bash
   nano xorcism.c
   gcc xorcism.c -o xorcism
   ```
2. Copy the source so the original program stays unchanged:
   ```bash
   cp xorcism.c decoder.c
   nano decoder.c
   ```
3. At the start of `main()` in `decoder.c`, before the original password input, add:
   ```c
   for (int i = 0; i < PWLEN; i++)
       putchar(enc[i] ^ key[i % 4]);
   putchar('\n');
   return 0;
   ```
4. Decode the password:
   ```bash
   gcc decoder.c -o decoder
   ./decoder
   ```
   Output: `pleas3_dOnT_FeED_7HE_nAs4L_DemoNs`.
5. Run the **original** program and enter this password:
   ```bash
   ./xorcism
   ```
   Submit the flag it prints.

**Why:** XOR with the same key restores the original byte. `i % 4` repeats the four-byte key.

**Accepted flag:** `cctf{pleas3_dOnT_FeED_7HE_nAs4L_DemoNs}`

## 3. cavern

The program frees a structure but still returns its old address.

1. Send `/challenge cavern`, then connect using the password supplied by the bot:
   ```bash
   ssh a.khaliullova-cavern@osctf.nnk.moe
   ```
2. In the container, create `offer.c`:
   ```bash
   nano offer.c
   ```
   Paste this complete code:
   ```c
   #include <stdio.h>
   #include <dlfcn.h>

   void run(void)
   {
       void *(*guess)(char *) = dlsym(RTLD_DEFAULT, "guess");
       char *p = guess("hello");
       printf("%.*s\n", 55, p + 64);
   }
   ```
3. Compile and run:
   ```bash
   gcc -shared -fPIC -o offer.so offer.c -ldl
   ./cavern ./offer.so
   ```
4. Submit the printed flag.

**Why:** `dlsym` finds `guess()`. Its structure starts with `guess[64]`, followed by `secret`. The old address plus 64 reaches the secret bytes. This reads freed memory, which is normally unsafe, but it worked here.
