# CCTF solutions by Daniil Chernykh

Most the solutions are hand-re-written-by-memory, so probably I did some typos or minor logical mistakes. Please just ignore them.

## Cavern

In source code function `void* guess(char*)` returns dangling pointer to the struct of two strings of 64 characters. We know, that first string is our previous guess and the next 64 characters is the already decyphered correct answer named `secret`. After accessing AND SAVING this `secret to another variable we may pass it to the cavern again and get success message. The answer is

```cctf{R3M3mb3R_tH3R3_is_n0_spO0n_Fr33_fR0m_naSal_DeM0NS}```

## Legacy


I used bit fields to imitate 18-bit value overflow.

```c
#include <limits.h>
#include <stdio.h>


struct please {
	unsigned work: 18;
};


struct please position(const char *s)
{
	struct please h;
        h.work = 0;
	while (*s) {
		h.work = h.work * 67 + *s++;
	}
	return h;
}

void diagnostics(void)
{
	printf("== Diagnostics: ==\n");
	printf(" struct please has %d bits\n", (int)(sizeof(struct please) * CHAR_BIT));
	printf(" check value: %u\n", position("X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*").work);
	printf("== End of Diagnostics ==\n\n");
}

void add_data(char* document, const char* data)
{
	struct please pos = position(data);
	document[pos.work] = *data;
}

void decrypt_document(void)
{
	char document[69] = {0};

	add_data(document, "C2KyY");
	add_data(document, "D0dit");
	add_data(document, "52Vor");
	add_data(document, "i0Xhz");
	add_data(document, "O0CDf");
	add_data(document, "y1TRq");
	add_data(document, "_0GVc");
	add_data(document, "_0GVl");
	add_data(document, "d02t9");
	add_data(document, "o0Ki8");
	add_data(document, "l0oTk");
	add_data(document, "_1yII");
	add_data(document, "g1A7g");
	add_data(document, "e0tqV");
	add_data(document, "g1A7T");
	add_data(document, "w0MrP");
	add_data(document, "d0mKc");
	add_data(document, "A1EUf");
	add_data(document, "d02tL");
	add_data(document, "_0GVq");
	add_data(document, "00BEh");
	add_data(document, "_0GW8");
	add_data(document, "_0GVh");
	add_data(document, "t07CX");
	add_data(document, "_0GVv");
	add_data(document, "T2WnM");
	add_data(document, "H1zSK");
	add_data(document, "l1foc");
	add_data(document, "O0CDc");
	add_data(document, "h2zOp");
	add_data(document, "o0KiK");
	add_data(document, "O0CDP");
	add_data(document, "D0djD");

	printf("== Decrypted Document: ==\n");
	printf("cctf{%s}\n", document);
	printf("== End of Decrypted Document ==\n\n");
}

int main()
{
	diagnostics();

	printf("Attempting to decrypt the document...\n");

	decrypt_document();

	return 0;
}
```

## Xorcism

Well, no need to guess, we just decrypt the encrypted flag.

```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
	0x67, 0x10, 0x3f, 0x3b, 0x44, 0x64, 0x05, 0x4b, 0x58, 0x4f,
	0x2e, 0x50, 0x51, 0x44, 0x3f, 0x6b, 0x68, 0x55, 0x12, 0x3c,
	0x68, 0x6f, 0x6e, 0x5c, 0x03, 0x10, 0x05, 0x6b, 0x52, 0x4c,
	0x6a, 0x41, 0x44
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main(void) {
	printf("The flag is: cctf{");
	for (int i = 0; i < PWLEN; i++) {
		printf("%c", enc[i] ^ key[i % 4]);
	}
	printf("}\n");
	return 0;
}
```

