# Finding Nemo — Threads CTF solutions

Author: Robert Kharisov
Session: 15–17 November 2026

The commands below identify whether they run on my Mac or inside the assigned container. Session-specific values such as case facts and suspect numbers must be checked again in a new run.

## decrypt

Each shard takes about ten seconds and may return no output even with exit status zero. I ran twelve copies per shard concurrently, kept the first nonempty stdout for each shard, retried missing pieces, and joined complete outputs in numeric shard order. This recovered all fifty pieces in about 10.5 seconds on my Mac, within the 30-second window. Implementation files below: `manager.py` and `run.sh`.

```bash
# On my Mac, from findingnemo/exploit, with the supplied decrapinator already downloaded:
./run.sh ~/Downloads/shards.tar.gz
```

**Result:** `threadctf{yOU_totalLY_WROt3_a_PropER_PROGraM_fOr_th1s_right}`. Each new challenge start supplies a new archive and flag; the manager is reused.

## discodetective

I connected to the dialogue service on `127.0.0.1:9999`. My Python client handles the three-second events automatically while selecting feasible interview questions. It builds `WARD` replies from the current strike, normalizes the action to the required word format, clears question numbers when the room changes, and pays only when `mind + 2 < 5`. At the final prompt it keeps processing socket events while accepting keyboard commands through `select`.

Implementation below: `det.py`; [successful transcript](../initial-materials/discodetective/transcript/07-success.txt). The client included below is reconstructed from my recorded code and final edits; the exact server-side file was not preserved or rerun during report preparation.

Save the client implementation below as `det.py` and copy it to `/tmp/det.py` inside the container, then run:

```bash
python3 -m py_compile /tmp/det.py
python3 -u /tmp/det.py | tee /tmp/discodetective.log
```

My case produced deploy time 02:35 (`f1`), the change ticket (`f2`), a badge map (`f3`), departure logs (`f4`), and witness statements. Aiko Mori claimed to have left at 01:36, but badge 5230 left at 03:07. Under the game’s rules she was the only suspect both present after deployment and lying about departure. I entered these commands in the same connected client:

```text
ACCUSE 2
PROOF f1 f3 f4
```

The proof uses deploy time, badge identity and actual departure; the already collected statements supply the conflicting claim. Suspect numbers and fact IDs refer to this particular case.

**Result:** `threadctf{pUt_oN_p4Nts_NOt_imP0st3r_Syndr0ME}`.

## fortforks

The `secrets` file is a FIFO. The challenge kills each process after five seconds and resets the stream if reading stops. I opened the FIFO once and handed the descriptor and buffered state to a new child about once per second using `fork()`. The parent exits immediately; the child continues from the same open endpoint. The reader retains a tail buffer and searches for the flag.

Implementation below: `reader.py`. Commands below run on my Mac from `findingnemo/exploit/fortforks`:

```bash
scp reader.py r.kharisov-fortforks@osctf.nnk.moe:/tmp/reader.py
ssh r.kharisov-fortforks@osctf.nnk.moe 'nohup python3 -u /tmp/reader.py /home/ctf/secrets </dev/null >/tmp/fortforks.log 2>&1 &'
# Reconnect after the reader has completed:
ssh r.kharisov-fortforks@osctf.nnk.moe 'tail -n 12 /tmp/fortforks.log'
```

**Result:** `threadctf{on1Y_Prop3R_LIn3a93_Get_T0_lIVe_1N_Ca5tl3}` after 2097243 bytes and 30 handoffs. [Saved output](../initial-materials/fortforks/success.txt). `nohup` and redirected streams handle SSH disconnection; the periodic fork handles the per-process lifetime. The reader assumes the flag is near the end, as stated in the task, and has an inherited 180-second deadline.


## Implementations

Save each block under its displayed filename before running the commands above. Original challenge files, libraries and binaries must be obtained from the course materials.

<details>
<summary>manager.py</summary>

```python
#!/usr/bin/env python3
"""
manager.py - decrypt all shards of the "decrypt" challenge in parallel.

The decrapinator is unreliable: each run takes ~10 s and may crash.  We launch
K independent copies per shard per round and keep the first success.  Shards
that fail all K copies are retried in the next round.  Everything runs in
parallel, so a round costs ~10 s regardless of shard count.

Usage:
    python3 manager.py <shards.tar.gz | shards_dir> [--k N] [--deadline S]
"""

import argparse
import glob
import os
import re
import subprocess
import sys
import tarfile
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

HERE = os.path.dirname(os.path.abspath(__file__))

# Candidate locations of the decrapinator, relative to this script.
# Candidate locations of the decrapinator, relative to this script.  The
# script may live next to the decrapinator (initial-materials/decrypt/) or in
# a sibling directory such as exploit/, so both layouts are searched.
DIRS = [
    HERE,
    os.path.join(HERE, "..", "initial-materials", "decrypt"),
]
NAMES = [
    "decrapinator-universal-darwin",
    "decrapinator-x86_64-linux",
    "decrapinator-aarch64-linux",
]
DECRAP_CANDIDATES = [
    os.path.join(d, "from-torrent", "Decrapinator.v2.Linux.MacOS.Cracked", n)
    for d in DIRS for n in NAMES
]


def find_decrap():
    env = os.environ.get("DECRAP")
    if env:
        return env
    for p in DECRAP_CANDIDATES:
        if os.path.isfile(p):
            return p
    sys.exit("decrapinator not found; set DECRAP=/path/to/decrapinator")


def prepare(path):
    """Return a directory containing the .shard files."""
    if os.path.isdir(path):
        return path
    if tarfile.is_tarfile(path):
        d = tempfile.mkdtemp(prefix="shards_")
        with tarfile.open(path) as t:
            t.extractall(d)
        return d
    sys.exit(f"not a directory or tar archive: {path}")


def shard_index(path):
    m = re.search(r"(\d+)", os.path.basename(path))
    return int(m.group(1)) if m else 0


def collect_shards(root):
    files = glob.glob(os.path.join(root, "**", "*.shard"), recursive=True)
    if not files:
        sys.exit(f"no .shard files under {root}")
    return sorted(files, key=shard_index)


def try_shard(decrap, shard, k):
    """Run k copies in parallel; return the first non-empty stdout, or None."""
    procs = []
    for _ in range(k):
        try:
            procs.append(subprocess.Popen(
                [decrap, "decrypt", shard],
                stdout=subprocess.PIPE,
                stderr=subprocess.DEVNULL,
            ))
        except OSError:
            pass
    outs = []
    for p in procs:
        try:
            out, _ = p.communicate(timeout=20)
        except subprocess.TimeoutExpired:
            p.kill()
            out = b""
        if p.returncode == 0 and out:
            outs.append(out)
    return outs[0] if outs else None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("input", help="shards.tar.gz or a directory with *.shard")
    ap.add_argument("--k", type=int, default=8, help="copies per shard per round")
    ap.add_argument("--deadline", type=float, default=25.0,
                    help="seconds from start after which to stop retrying")
    args = ap.parse_args()

    decrap = find_decrap()
    root = prepare(args.input)
    shards = collect_shards(root)
    t0 = time.time()

    print(f"[*] {len(shards)} shards, k={args.k}, deadline={args.deadline}s",
          file=sys.stderr)

    results = {}
    pending = list(shards)
    rnd = 0
    while pending and (time.time() - t0) < args.deadline:
        rnd += 1
        print(f"[*] round {rnd}: {len(pending)} shards", file=sys.stderr)
        with ThreadPoolExecutor(max_workers=len(pending)) as ex:
            futs = {ex.submit(try_shard, decrap, s, args.k): s for s in pending}
            for f in as_completed(futs):
                s = futs[f]
                r = f.result()
                if r:
                    results[s] = r
        pending = [s for s in pending if s not in results]
        print(f"[*] round {rnd} done: {len(results)}/{len(shards)} "
              f"({time.time() - t0:.1f}s)", file=sys.stderr)

    if pending:
        print(f"[!] {len(pending)} shards unresolved: "
              f"{[os.path.basename(s) for s in pending]}", file=sys.stderr)
        sys.exit(2)

    flag = b"".join(results[s] for s in shards).decode("utf-8", "replace")
    print(flag)
    print(f"[*] total {time.time() - t0:.1f}s", file=sys.stderr)


if __name__ == "__main__":
    main()
```

</details>

<details>
<summary>run.sh</summary>

```bash
#!/bin/bash
# run.sh - one command to decrypt the newest shards archive from ~/Downloads.
#
# Usage:
#   ./run.sh                  # use newest ~/Downloads/shards*.tar.gz
#   ./run.sh /path/to/shards.tar.gz
set -e
cd "$(dirname "$0")"

if [ -n "$1" ]; then
    f="$1"
else
    f=$(ls -t "$HOME"/Downloads/shards*.tar.gz 2>/dev/null | head -1 || true)
fi

if [ -z "$f" ] || [ ! -f "$f" ]; then
    echo "usage: $0 [shards.tar.gz]" >&2
    echo "no shards*.tar.gz found in ~/Downloads" >&2
    exit 1
fi

echo "[*] using $f" >&2
K=${K:-12}
exec python3 manager.py "$f" --k "$K" --deadline 25
```

</details>

<details>
<summary>det.py</summary>

```python
# Reconstructed from the posted det.py and the edits confirmed in the chat.
# The exact final /tmp/det.py was not downloaded from the challenge container.
# This preserves the session implementation, including its documented limitations.
import socket, re, sys, time
import select

manual = False

HOST, PORT = "127.0.0.1", 9999
VERB = {"COMPULSION": "FORBID", "PARANOIA": "REBUKE", "MORTIFICATION": "DISMISS",
        "EUPHORIA": "REFUSE", "MALAISE": "DEFY"}

def form(kind, action):
    w = re.findall(r"[a-z0-9]+", action.lower())
    if kind == "reversed":
        w.reverse()
    s = "-".join(w)
    return s.upper() if kind == "shout" else s

# Приоритет: какой факт нужен для PROOF
WANT = [
    ("counter first go below zero", 100),   # deploy-time
    ("who asked you to ship", 95),          # deploy-path
    ("access roster", 92),                  # key-holders
    ("planner", 90),                        # key-holders
    ("written statements", 85),             # statements
    ("carbon copy", 80),                    # badge-log
    ("pizza order", 75),                    # badge-map
    ("core dump", 60),                      # change
]

s = socket.create_connection((HOST, PORT), timeout=10)
s.settimeout(0.15)
buf = b""
skills, options, mind = {}, [], 1
last, asked = {}, set()

def send(x):
    print(">>>", x, flush=True)
    s.sendall((x + "\n").encode())

def pick():
    """Выбрать лучший ещё не заданный вопрос: приоритет факта, потом запас."""
    best = None
    for n, sk, diff, text in options:
        if n in asked:
            continue
        gap = diff - (skills.get(sk, 0) + 5)
        prio = 0
        for kw, p in WANT:
            if kw in text:
                prio = p
                break
        # провальный вариант без ценности не берём вовсе
        if gap > 2 and prio == 0:
            continue
        # при равном приоритете предпочитаем проходящий без доплаты
        key = (gap <= 0, gap <= 2, prio, -gap)
        if best is None or key > best[0]:
            best = (key, n, gap, prio)
    if best is None:
        return None
    print(f"### pick {best[1]} gap={best[2]} prio={best[3]} mind={mind}", flush=True)
    return best[1]

while True:
    if manual and select.select([sys.stdin], [], [], 0)[0]:
        command = sys.stdin.readline()
        if command:
            send(command.strip())
        else:
            manual = False
    try:
        c = s.recv(65536)
        if not c:
            break
        buf += c
    except socket.timeout:
        pass
    t = buf.decode("utf-8", "replace")
    while "\n" in t:
        line, t = t.split("\n", 1)
        print(line, flush=True)

        if line.startswith("!!SCENE"):
            asked.clear()
            options.clear()

        m = re.search(r"!!SHEET (.+)$", line)
        if m:
            for kv in m.group(1).split():
                k, v = kv.split("=")
                skills[k] = int(v)

        m = re.search(r"!!MIND count=(\d+)", line)
        if m:
            mind = int(m.group(1))

        # каждый strike -> WARD, форма берётся из самой строки
        m = re.search(r'!!THOUGHT-STRIKE sigil=(\S+) type=\S+ flavor=(\S+) verb=(\S+) form=(\S+) action="([^"]*)"', line)
        if m:
            sigil, flavor, verb, kind, act = m.groups()
            now = time.time()
            if now - last.get(sigil, 0) > 0.8:
                last[sigil] = now
                send(f"WARD {sigil} {VERB.get(flavor, verb)} {form(kind, act)}")

        if line.startswith("!!QUESTION-LOCKED"):
            m2 = re.search(r"n=(\d+)", line)
            if m2:
                asked.add(int(m2.group(1)))

        # новый блок вариантов начинается с !!OPTIONS
        if line.startswith("!!OPTIONS"):
            options = []

        m = re.match(r"\s*(\d+)\) \[(\w+): \w+ (\d+)\] (.*)", line)
        if m:
            n = int(m.group(1))
            if "asked=1" in line:
                asked.add(n)
            else:
                options.append((n, m.group(2), int(m.group(3)), m.group(4)))

        if line.startswith("!!PICK"):
            n = pick()
            options = []
            if n is None:
                send("LEAVE")
            else:
                asked.add(n)
                send(str(n))

        if line.startswith("!!PROMPT short"):
            if "PAY or ACCEPT" in line and mind + 2 < 5:
                send("PAY")
            else:
                send("ACCEPT")

        if line.startswith("!!PROMPT accuse"):
            manual = True
            print("### Ручной режим. Вводи команды игры и нажимай Enter.", flush=True)
        if line.startswith("!!GAME-OVER") or "cctf{" in line:
            sys.exit(0)
    buf = t.encode()
```

</details>

<details>
<summary>reader.py</summary>

```python
"""Read one FIFO while replacing the reader process every second."""

import os
import re
import select
import sys
import time


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "/home/ctf/secrets"
    deadline = time.monotonic() + 180
    handoff_at = time.monotonic() + 1
    report_at = time.monotonic() + 5
    tail = b""
    total = 0
    received = False
    generation = 0
    flag_pattern = re.compile(rb"threadctf\{[^}\r\n]{1,512}\}")

    # Opening nonblocking lets us begin even if the writer is not ready yet.
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK)
    print(f"Opened {path}; pid={os.getpid()}", flush=True)

    try:
        while time.monotonic() < deadline:
            now = time.monotonic()
            if now >= handoff_at:
                child = os.fork()
                if child:
                    # The child already holds the same open FIFO descriptor.
                    # _exit avoids flushing copied Python output buffers.
                    os._exit(0)
                generation += 1
                handoff_at = time.monotonic() + 1

            if now >= report_at:
                print(f"bytes={total} generation={generation} pid={os.getpid()}",
                      flush=True)
                report_at = now + 5

            ready, _, _ = select.select([fd], [], [], 0.05)
            if not ready:
                continue
            try:
                data = os.read(fd, 65536)
            except BlockingIOError:
                continue

            if not data:
                if received:
                    print("EOF reached without a recognized flag. Last bytes:",
                          flush=True)
                    print(repr(tail), flush=True)
                    return 2
                # No writer yet. Avoid busy-waiting on an idle FIFO.
                time.sleep(0.05)
                continue

            received = True
            total += len(data)
            tail = (tail + data)[-4096:]
            match = flag_pattern.search(tail)
            if match:
                print("FLAG " + match.group().decode("ascii"), flush=True)
                print(f"bytes={total} generation={generation}", flush=True)
                return 0

        print("Stopped after 180 seconds. Last bytes:", flush=True)
        print(repr(tail), flush=True)
        return 3
    finally:
        os.close(fd)


if __name__ == "__main__":
    sys.exit(main())
```

</details>
