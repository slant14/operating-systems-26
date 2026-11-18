# threadctf — princess2 (125 points)

**Flag:** `threadctf{i_hoP3_you_Can_C_CL3aRLy_noW}`

## Task

Container: `ssh t.zainullin-princess2@osctf.nnk.moe`. The story repeats the
`princess` hunt, but this time the forest kept the shell itself. The login
shell is a restricted **story-shell**:

```
The forest kept your shell. It won't give it back.
All you have left is gcc and whatever you compile.
Allowed: gcc, vim/nano, rm and ./binaries
Forbidden: Everything else
```

`ps`, `kill`, `ls`, `cat`, `echo` are all blocked. This time **100** cursed
cabins must be torn down, and the tool to find/kill them has to be **written
in C** ("we can C the way", "we'll make our own blade").

## Solution — `blade.c`

A self-contained hunter:

- walks `/proc/*/stat`, parses `pid (comm) state ppid`;
- a zombie princess is `state == 'Z'` with `comm` starting with `princess_`;
- its parent (`ppid > 1`) is the cursed cabin → `kill(ppid, SIGTERM)`;
- keeps sweeping every second, and as soon as the forest drops
  `/home/ctf/flag.txt`, prints the flag and exits.

Notes on the environment:

- SFTP does not work through the story-shell — the file has to be created
  inside (e.g. via `vim`/`nano`);
- the hunt takes a while (~100 kills), so it is best run in the background of
  your own terminal (`./blade &`) or just left running.

```
gcc blade.c -o blade
./blade
...
killed cursed cabin 1218 (total 100)

=== The Forest ===
The forest falls silent. 100 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.

FLAG FOUND after 100 kills: threadctf{i_hoP3_you_Can_C_CL3aRLy_noW}
```

Send the flag as text to the bot in the `findingnemo` [threadctf] instance.

## Result

```
✅ Flag accepted for 'princess2'.   125 points
```
