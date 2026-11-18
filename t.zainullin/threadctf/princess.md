# threadctf — princess (100 points)

**Flag:** `threadctf{WH4t_do_YoU_m3An_th3Y_aRe_AlR34DY_DE4D}`

## Task

Container: `ssh t.zainullin-princess@osctf.nnk.moe`. Inside, PID 1 is a
root-owned process `/root/forest` that maintains a population of processes:

- cabins: `the_<adjective>_cabin princess_<name>` — each spawns a child
  princess process with the same name;
- princesses: `princess_<name>`;
- items: `the_locket`, `the_crown`, `the_blade`, ... (plain sleeping processes).

Some princesses are already dead — they appear as zombies
(`[princess_heart] <defunct>`), their cabin is still alive. The story text says:

> "It's the cabins that are cursed, not the princesses. Tear down **fifty** of
> the cursed ones... **don't touch the living ones. Or their cabins.**"

## Idea

Kill **only the cabins whose princess child is a zombie** (the cursed ones).
The forest replaces every killed cabin with a new one (randomly alive or
cursed), so this becomes a hunt: wait for cursed cabins to appear, kill them,
repeat 50 times. Killing a living princess or her cabin would break the rule.

## Solution

Background hunter script (`/tmp/hunt.sh`), started via a background job:

```sh
#!/bin/sh
KILLED=0
while [ "$KILLED" -lt 60 ]; do
    TARGETS=$(ps -eo pid,ppid,stat,args --no-headers |
              awk '$3 ~ /^Z/ && $4 ~ /princess_/ {print $2}')
    if [ -z "$TARGETS" ]; then sleep 1; continue; fi
    for c in $TARGETS; do
        if kill "$c" 2>/dev/null; then
            KILLED=$((KILLED+1))
            echo "killed cabin $c (total $KILLED)"
        fi
    done
    sleep 1
done
```

Notes:

- zombie processes print as `[princess_heart] <defunct>`, so the awk filter
  must match `/princess_/` anywhere in args, not `^princess_`;
- the forest spawns cursed cabins slower over time — the whole run took
  ~15 minutes for ~50 kills.

When the counter reached 50, the forest dropped the reward into our home
directory:

```
cat /home/ctf/flag.txt
# threadctf{WH4t_do_YoU_m3An_th3Y_aRe_AlR34DY_DE4D}
```

Send the flag to the bot (in the `findingnemo` [threadctf] instance!).

## Result

```
✅ Flag accepted for 'princess'.   100 points
```
