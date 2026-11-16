the c code uses a hash function in position computation, which is supposed to store h value in 18-bit variable.

We create struct with unsigned int variable : 18, and use it for the computations
```c
struct bit_unsighed{
	unsigned int value : 18;
};

  

unsigned position(const char *s)
{
	struct bit_unsighed h;
	h.value = 0;

	while (*s) {
		h.value = h.value * 67 + (unsigned int)*s++;
	}
	
	return h.value;

}
```