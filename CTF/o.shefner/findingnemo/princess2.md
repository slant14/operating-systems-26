# `princess2` — C solution

Container: `o.shefner-princess2@osctf.nnk.moe`. On login, `story-shell` runs instead of a regular shell and allows only `gcc`, editors, `cat`, `rm`, and user-built binaries.

Inspecting the processes using C showed that both living and dead princesses have names matching `princess_*`. A dead princess has state `Z` in `/proc/<pid>/status`; the `PPid` field points to her cabin. The cabin's name must be read from `/proc/<PPid>/comm`: it may be `cabin_by_lake` or, for example, `the_fog_cabin`, so the check looks for `cabin` anywhere in the name, not just at the beginning.

The [source code](princess2_blade.c) repeatedly scans `/proc`, verifies the `Z` state, the princess's name, and the parent's name, then sends `SIGTERM` only to a matching cabin. After each pass, it checks `/home/ctf` for a small reward file.

The source can be passed directly to `gcc` through standard input. In `story-shell`:

```text
gcc -x c -o blade -
```

Then paste the contents of `princess2_blade.c`, press `Ctrl+D` on a new line, and run `./blade`. For an extended wait, use the [wrapper](princess2_daemon.c): it calls `fork` and `setsid`, redirects output to `/home/ctf/princess2.log`, and launches `blade`. Compile it the same way, then run `./daemon`; progress can be read with the permitted command `cat princess2.log`.

The first short run recognized only cabin names beginning with `cabin`; the corrected version also handles `the_fog_cabin` and similar names. In total, several short runs and one background run destroyed 100 cabins containing princesses in state `Z`. The program detected the reward in `/home/ctf/flag.txt`; a separate `cat flag.txt` check confirmed:

```text
threadctf{i_hOPE_YOu_can_C_cLEarLy_N0W}
```
