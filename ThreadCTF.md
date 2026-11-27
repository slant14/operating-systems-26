n.ionov@innopolis.university CSE-04

# ThreadCTF

## needle

`meeting` is a setuid program. It starts a thread `retrieval` that reads
`/root/flag.txt`, sleeps one second and returns a pointer to the flag string.
Then it loads `./libmeeting.so` and calls `meeting()` from the library. `main`
joins `retrieval` only after five seconds.

The plugin runs in the same process and sees the global `retrieval`, so it can
join the thread first and take the returned pointer (`sources/meeting.c`):

```c
extern pthread_t retrieval;

void *meeting(void *arg)
{
    void *fabric = NULL;
    if (pthread_join(retrieval, &fabric) == 0 && fabric != NULL)
        printf("%s\n", (char *)fabric);
    return NULL;
}
```

```bash
gcc -fPIC -shared -o libmeeting.so meeting.c -lpthread
./meeting
```

The second join in `main` fails and the program complains about the missing
fabric, but the flag is already printed.

Flag: `threadctf{5hoOting_5OmE_4toMic_F3et}`

## fortforks

`/home/ctf/secrets` is a FIFO, not a normal file. A read on it waits for a
writer and returns the stream piece by piece. The problem is the session limit:
SSH closes after about five seconds and kills the child processes, while the
flag appears later in the stream.

The reader opens the FIFO, forks twice, calls `setsid()` and continues to fork
every few seconds, so it is not a child of the SSH session and lives longer
than it. It searches the stream for `threadctf{` and writes the flag to
`/tmp/fortforks.flag` (`sources/forkreader.c`).

```bash
gcc -O2 -o /tmp/fr forkreader.c
( setsid /tmp/fr >/dev/null 2>&1 & )
sleep 20
cat /tmp/fortforks.flag
```

The prefix is `threadctf{`, not `cctf{`. With the wrong prefix the search never
matches, even if the flag is already in the stream.

Flag: `threadctf{oNlY_PrOper_Lineage_GEt_T0_l1ve_IN_cAstlE}`

## discodetective

The game listens on `127.0.0.1:9999` and talks in `!!` messages: `!!SHEET`
with the skills, `!!SUSPECT`, `!!SCENE`, `!!FACT`, `!!CLAIM`,
`!!THOUGHT-STRIKE`, `!!PROMPT` and `!!GAME-OVER`.

The timed part is `!!THOUGHT-STRIKE`: it must be answered with
`WARD <sigil> <verb> <action>` before the window ends, and the action is
transformed depending on the form (`reversed` reverses the word order, `shout`
uses upper case, `kebab` joins the words with a dash). In `MASKED` the verb and
the action are taken from the strike message.

The rest is deduction. Facts narrow the suspects: who worked on the change, who
holds the deploy key, who was in the building at the deploy time, and who lied
about the time of leaving. When one suspect is left and the case is closable,
the answer is `ACCUSE <n>` followed by `PROOF <fact-id> ...`.

A person cannot keep up with the timer, so I used a Python client (`solve7.py`
in the container). It answers `WARD` immediately, collects verified facts and
intersects the four sets until one person remains.

Flag: `threadctf{pu7_on_PaNts_Not_imPOster_SYndRom3}`
