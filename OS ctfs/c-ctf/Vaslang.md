The challenge gives two files. vaslang.c is a small stack-based interpreter.
inp.vas is a program written in that language: 1959 function definitions and
one final command, main!. Running it with the original interpreter mostly
prints errors like "func tender_749 undefined" and never reaches the flag.

The problem is the order of the definitions. They are not sorted by
dependency: many functions call functions that are defined later in the file.

I parsed inp.vas and counted 2493 such forward references. Every called name
exists somewhere in the file, and the call graph has no cycles, so the program

itself is valid. The interpreter is what is broken.
In xdef(), when the parser sees a call term, it tries to resolve the callee

immediately, using only the functions that have been defined so far:
```c
    } else {
        ftermp->ty = 'C';
        for (int i = 0; funcs + i < funcp; i++)
            if (strcmp(funcs[i].name, term) == 0)
                ftermp->target = funcs[i].body;

        if (ftermp->target == NULL)
            errorf("func %s undefined", term);

    }
```

If the callee is below the current line, the lookup fails. errorf() returns
from xdef() in the middle of the definition, the function is not added
properly, and everything that depends on it breaks too.

The fix is to resolve names in a second pass. While parsing, a call is stored
as a term of type 'c' and the function name is saved with strdup():

```c
    } else {
        ftermp->ty = 'c';
        ftermp->target = (struct term *)strdup(term);
    }
```
    

After all definitions have been read, resolve_calls() walks the term array,
finds every 'c', looks the name up in the finished function table and changes
the term into a normal 'C' call term:

```c

    void resolve_calls(void)
    {
        for (struct term *t = terms; t < termp; t++) {
            if (t->ty != 'c')
                continue;

            char *name = (char *)t->target;
            struct term *body = NULL;
            for (int i = 0; funcs + i < funcp; i++)
                if (strcmp(funcs[i].name, name) == 0) {
                    body = funcs[i].body;
                    break;
                }

            if (body == NULL)
                errorf("func %s undefined", name);
            t->target = body;
            t->ty = 'C';
        }
    }
```


xcmd() calls resolve_calls() once before run(). At that point every
definition has been parsed, so all forward references can be resolved.