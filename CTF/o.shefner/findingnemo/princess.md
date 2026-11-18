# CTF `princess` — report

**Instance:** `findingnemo` (`threadctf`).
**Challenge:** `princess`.
**Environment:** a training Linux container, user `ctf`.

## Discovery

The home directory `/home/ctf` initially contained only shell configuration files. The challenge entities were found among the processes:

```bash
pwd
ls -la
ls -la /
ps -ef
```

In the process list, `PID 1` is `/root/forest`. Its child processes are cabins. Each cabin has a princess child process. A dead princess appears as `[princess_name] <defunct>`; its state is `Z` (zombie). The `PPID` field points to the cabin's PID.

An example from the observed output:

```text
PID  PPID  CMD
10   1     cabin_in_pines princess_thorn
20   10    [princess_thorn] <defunct>
```

Here, the relevant cabin has PID `10`. Process IDs change, so they must be looked up again before each action:

```bash
ps -eo pid=,ppid=,stat=,comm= |
    awk '$3 ~ /^Z/ && $4 ~ /^princess_/ {print $2}' |
    sort -nu
```

## Solution

The following loop rereads the process list, verifies that the identified parent is still a cabin with a zombie princess, and sends `TERM` only to that parent. It stops after 50 successful signal sends.

```bash
count=0

while [ "$count" -lt 50 ]; do
    found=0

    while read -r cabin; do
        ps -p "$cabin" -o comm= 2>/dev/null | grep -q cabin || continue

        ps -eo ppid=,stat=,comm= |
            awk -v p="$cabin" '$1 == p && $2 ~ /^Z/ && $3 ~ /^princess_/ {found=1} END {exit !found}' ||
            continue

        if kill -TERM "$cabin" 2>/dev/null; then
            count=$((count + 1))
            found=1
            echo "Destroyed: $count/50 (PID $cabin)"
        fi

        [ "$count" -ge 50 ] && break
    done < <(
        ps -eo ppid=,stat=,comm= |
            awk '$2 ~ /^Z/ && $3 ~ /^princess_/ {print $1}' |
            sort -nu
    )

    [ "$count" -ge 50 ] && break
    [ "$found" -eq 0 ] && sleep 0.2
done
```

The container confirmed the result with this message:

> The forest falls silent. 50 cursed cabins lie in ruin, and
> the story is finally over. Your reward waits in your home
> directory.

**Reward location:** `/home/ctf`. The filename and the flag text are absent from the saved output, so they are not included here.
