[i.birkina@innopolis.university](mailto:i.birkina@innopolis.university) CSE-02

# СCTF

## 1. Cavern

I was given `cavern.c`. The program loads a shared library and runs the `run()` function from it.

First, I created `offer.c` with a function that printed "Hello from offer!" to check if my code worked.

```bash
nano offer.c
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

The program printed `Hello from offer!`, so my library worked.

Then I looked at `cavern.c` and noticed that the `guess()` function frees memory but still returns a pointer to it:

```c
offering_free(offering);
return offering;
```

This is a Use-After-Free vulnerability. I decided to try reading the secret from this memory.

First, I tried to find the `guess()` function:

```bash
nm -D ./cavern | grep guess
```

But I got `Permission denied`. So I used `dlsym()` inside `offer.c` instead. After recompiling, the program printed `Found guess function!`.

Then I changed `offer.c` to call `guess("wrong")` and read the secret:

```c
char *p = guess("wrong");
char flag[56];

for (int i = 0; i < 55; i++) {
    flag[i] = ((volatile char *)p)[64 + i];
}

flag[55] = '\0';
printf("FLAG: %s\n", flag);
```

I started from byte 64 because the first 64 bytes stored the guess, and the next bytes stored the secret.

Finally, I compiled and ran the program again:

```bash
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

The program printed the flag.

---

## 2. Legacy

I was given an old C program `legacy.c` that crashed on modern computers.

The task included these diagnostics:

```
unsigned has 18 bits
check value: 237614
```

I looked at the code and found the `position()` function. It calculates a number from a string and uses it as an array index.

The problem was that the original computer used 18-bit unsigned integers, but my Mac uses 32-bit unsigned integers. Because of this, the calculated indices could become too large.

I changed this line:

```c
h = h * 67 + *s++;
```

To:

```c
h = (h * 67 + (unsigned char)*s++) & 0x3FFFFu;
```

The mask `& 0x3FFFFu` keeps only the lowest 18 bits, so the calculations work like on the old computer.

Then I compiled and ran the program:

```bash
cd ~/Downloads
gcc legacy.c -o legacy
./legacy
```

The program printed:

```
unsigned has 32 bits
check value: 237614
```

The check value matched the original diagnostics. After that, the program successfully decrypted the document and printed the flag.

---

## 3. Vaslang

I was given `vaslang.c`, an interpreter, and `inp.vas`, a program that should print the flag.

First, I checked the interpreter code. I found that some functions in `inp.vas` were called before they were defined. The interpreter tried to find them immediately and returned `ERROR: func ... undefined`.

To fix this, I changed the code to save function names instead of searching for them immediately:

```c
ftermp->ty = 'U';
ftermp->target = (struct term *)strdup(term);
```

Here, `U` means that the function has not been resolved yet.

Then I added `resolve_calls()` to find the saved functions after their definitions were loaded.

When a function was found, its name was replaced with its address:

```c
t->target = target;
t->ty = 'C';
```

I also added `resolve_calls()` inside `xcmd()` before execution.

Next, I replaced:

```c
*strchrnul(buf, '\n') = 0;
```

With:

```c
char *nl = strchr(buf, '\n');
if (nl != NULL)
    *nl = 0;
```

I did this because `strchrnul()` was not supported on my Mac. The new code removes the newline character if it exists.

After these changes, I compiled the corrected interpreter and ran `inp.vas`:

```bash
cd ~/Downloads
gcc -O2 vaslang_fixed.c -o vaslang
./vaslang < inp.vas | grep -o 'cctf{[^}]*}'
```

The `< inp.vas` part passes the file to the interpreter, and `grep` extracts the flag from the output.

The corrected program successfully printed the flag.
