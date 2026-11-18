# threadctf — fortforks (294 points)

**Flag:** `threadctf{on1y_PrOpeR_L1n3AGe_9e7_t0_LIVe_In_Ca57le}`

## Task

Container: `ssh t.zainullin-fortforks@osctf.nnk.moe` (paramiko works fine for
connecting; the story tells the whole challenge):

> "...their secrets are all inside a pipe... what we really want is surely at
> the end of it... tons upon tons of garbage, which takes just ages to read...
> no process we create can survive longer than 5 seconds there before being
> killed — not even our own ssh session. And if you stop reading — you have to
> start over... mumbled something about **eating utensils**..."

Facts inside the container:

- `~/secrets` is a **named pipe (FIFO)**;
- PID 8: `python3 /root/forks.py` — the writer (writes garbage ~85 KB/s, the
  flag is at the very end, ~2 MB in total);
- PID 9: `python3 /root/limited_killer.py` — kills any process of the ctf user
  that lives longer than ~5 seconds (so a plain `sleep 7` dies silently);
- `/root/*.py` are root-only, the trick must be derived from the story.

## Solution — immortal fork chain reader

The FIFO fd is inherited by `fork()`, so a chain of processes, each forked
every ~4 seconds (before the 5s kill), keeps the pipe open forever: the writer
never sees a closed reader, and reading continues seamlessly. Each link is
younger than 5s, so the killer never gets it ("proper lineage" indeed).

`/tmp/reader.c` (compiled with the in-container gcc):

- opens `/home/ctf/secrets`;
- keeps a rolling tail of the last 8192 bytes read;
- every 4 seconds saves `total=...` to `/tmp/r.state` and `fork()`s — parent
  exits, child continues reading the inherited fd;
- on EOF (writer closes the pipe) dumps the tail to `/tmp/out.txt` and marks
  `done=1`.

Run it detached:

```
gcc -O2 -o /tmp/reader /tmp/reader.c
setsid /tmp/reader &
```

Monitor `/tmp/r.state` from fresh (short-lived) ssh sessions; after ~2 minutes:

```
total=2097243 done=1
```

and `/tmp/out.txt` ends with:

```
Congratulations, Agent. The flag is: threadctf{on1y_PrOpeR_L1n3AGe_9e7_t0_LIVe_In_Ca57le}
```

## Result

```
✅ Flag accepted for 'fortforks'.   294 points
```
