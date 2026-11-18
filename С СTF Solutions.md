# Cavern

The `offering` struct contains `guess` and `secret`. The `guess` function returns a pointer to the beginning of this struct. To get the secret, we use this pointer.
```
#include <stdio.h>

extern void* guess(char*);

void run(void){
	char* my_guess = guess("cheburashka");
	for (int i = 0; i < 64; i++){
	    printf("%c", my_guess[64 + i]);
	}
}
```

# Legacy

Based on the diagnostics, the developer and I have different sizes of `unsigned`. They behave differently on overflow: in my case the mask is taken modulo 2^32, while in the developer's case it's modulo 2^18. Let's simulate the program's behavior as it is on the developer's side by taking the mask `0x3FFFF`.

```
unsigned position(const char *s)
{
    uint16_t h = 0;
    while (*s) {
        h = (h * 67 + *s++) & 0x3FFFF;
    }
    return h;
}
```

# Xorcism

The `enc[]` array contains the encrypted flag, and the `key` array is needed to decrypt it. Decryption is done using XOR.
```
int main(void)
{
    char in[64] = {0};
    for (int i = 0; i < PWLEN; i++) {
        in[i] = enc[i] ^ key[i % 4];
    }
    printf("correct! flag: cctf{%s}\n", in);
    return 0;
}

```