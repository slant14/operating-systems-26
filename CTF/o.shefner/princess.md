# Threads CTF: princess

Instance: `findingnemo` (`threadctf`). Challenge: `princess`.

## Location and observation

The challenge container was accessed over SSH. The working directory was
`/home/ctf`; it initially contained only shell startup files. The relevant
objects were Linux processes, not files in the home directory.

```bash
pwd
ls -la
ls -la /
ps -ef
```

The process list showed `/root/forest` as PID 1. Cabin processes were its
children. Each cabin had a princess process as a child. A dead princess
appeared as `[princess_name] <defunct>` with zombie state `Z`.

For example, one snapshot contained:

```text
PID  PPID  CMD
10   1     cabin_in_pines princess_thorn
20   10    [princess_thorn] <defunct>
```

The princess's PPID (`10`) identified the cabin to terminate. The list of
PIDs changed while the challenge ran, so fixed PIDs could not be used.

To list current cabin PIDs with zombie princesses:

```bash
ps -eo pid=,ppid=,stat=,comm= |
    awk '$3 ~ /^Z/ && $4 ~ /^princess_/ {print $2}' |
    sort -nu
```

## Solution

For each candidate, check that the parent still exists, that it is a cabin,
and that it still has a zombie princess. Then terminate that parent. Repeat
until 50 successful signals have been sent. The process list is refreshed on
each pass because the forest creates new cabins.

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

After the fiftieth cabin, the container reported:

> The forest falls silent. 50 cursed cabins lie in ruin, and the story is
> finally over. Your reward waits in your home directory.

The reward location was therefore `/home/ctf`. The flag text was submitted
to the CTF bot. Its exact text is omitted from this report.
