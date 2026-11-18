#!/usr/bin/env python3
"""Keep one FIFO reader alive by handing its open descriptor to fresh forks."""
import errno
import os
from pathlib import Path
import signal
import sys
import time

PIPE = Path(sys.argv[1] if len(sys.argv) > 1 else '/home/ctf/secrets')
RESULT = Path(sys.argv[2] if len(sys.argv) > 2 else '/home/ctf/fortforks.result')
LOG = Path(sys.argv[3] if len(sys.argv) > 3 else '/home/ctf/fortforks.log')
TAIL_LIMIT = 128 * 1024


def log(message):
    with LOG.open('a', encoding='utf-8') as f:
        f.write(f'{time.strftime("%H:%M:%S")} {message}\n')


def run():
    signal.signal(signal.SIGHUP, signal.SIG_IGN)
    # Open exactly once. All descendants inherit this same FIFO reader.
    fd = os.open(PIPE, os.O_RDONLY | os.O_NONBLOCK)
    tail = bytearray()
    total = 0
    started = False
    empty_since = None
    born = time.monotonic()
    next_log = born + 5

    while True:
        now = time.monotonic()
        if now - born >= 1.5:
            child = os.fork()
            if child:
                os._exit(0)
            born = time.monotonic()

        try:
            data = os.read(fd, 65536)
        except BlockingIOError:
            data = None
        except InterruptedError:
            continue

        if data:
            started = True
            empty_since = None
            total += len(data)
            tail.extend(data)
            if len(tail) > TAIL_LIMIT:
                del tail[:-TAIL_LIMIT]
        elif data == b'' and started:
            if empty_since is None:
                empty_since = now
            elif now - empty_since >= 1.0:
                RESULT.write_bytes(tail)
                log(f'EOF: read {total} bytes; saved final {len(tail)} bytes to {RESULT}')
                return

        if now >= next_log:
            log(f'read {total} bytes so far')
            next_log = now + 5
        time.sleep(0.02)


if __name__ == '__main__':
    # Detach from the short-lived SSH login before starting the reader.
    pid = os.fork()
    if pid:
        print(f'Started reader PID {pid}; check {LOG} and {RESULT}')
        raise SystemExit(0)
    os.setsid()
    devnull = os.open(os.devnull, os.O_RDWR)
    for target in (0, 1, 2):
        os.dup2(devnull, target)
    if devnull > 2:
        os.close(devnull)
    try:
        log(f'started; FIFO={PIPE}')
        run()
    except BaseException as exc:
        log(f'ERROR: {type(exc).__name__}: {exc}')
        os._exit(1)
