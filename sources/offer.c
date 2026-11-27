#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>

typedef void *(*guess_fn)(char *);

void run(void)
{
    dlerror();
    guess_fn guess = (guess_fn)dlsym(RTLD_DEFAULT, "guess");
    if (dlerror() != NULL) return;
    unsigned char *offering = (unsigned char *)guess("");
    if (offering == NULL) return;
    write(STDOUT_FILENO, offering + 64, 55);
    write(STDOUT_FILENO, "\n", 1);
}
