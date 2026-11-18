#!/usr/bin/env python3
"""Interactive client for the local CTF game; answers THOUGHT-STRIKE automatically."""
import re
import selectors
import socket
import sys


def make_ward(line):
    if not line.startswith('!!THOUGHT-STRIKE '):
        return None
    fields = dict(re.findall(r'\b(sigil|verb2?|form2?)=([^\s]+)', line))
    match = re.search(r'action="([^"]*)"', line)
    if not match or not {'sigil', 'verb', 'form'} <= fields.keys():
        raise ValueError(f'Cannot parse strike: {line}')
    def one_ward(action, verb, form):
        words = [word.lower() for word in re.findall(r'[A-Za-z0-9]+', action)]
        if form == 'reversed':
            words.reverse()
        elif form == 'shout':
            words = [word.upper() for word in words]
        elif form != 'kebab':
            raise ValueError(f'Unknown form: {form}')
        return f'WARD {fields["sigil"]} {verb} {"-".join(words)}\n'

    answer = one_ward(match.group(1), fields['verb'], fields['form'])
    if 'type=SPLIT' in line:
        second = re.search(r'action2="([^"]*)"', line)
        if not second or 'verb2' not in fields or 'form2' not in fields:
            raise ValueError(f'Cannot parse split strike: {line}')
        answer += one_ward(second.group(1), fields['verb2'], fields['form2'])
    return answer


def main():
    sock = socket.create_connection(('127.0.0.1', 9999), timeout=5)
    sock.setblocking(False)
    selector = selectors.DefaultSelector()
    selector.register(sock, selectors.EVENT_READ)
    selector.register(sys.stdin, selectors.EVENT_READ)
    pending = b''
    try:
        while True:
            for key, _ in selector.select():
                if key.fileobj is sys.stdin:
                    line = sys.stdin.buffer.readline()
                    if not line:
                        return 0
                    sock.sendall(line)
                    continue
                chunk = sock.recv(65536)
                if not chunk:
                    if pending:
                        sys.stdout.buffer.write(pending)
                        sys.stdout.buffer.flush()
                    return 0
                sys.stdout.buffer.write(chunk)
                sys.stdout.buffer.flush()
                pending += chunk
                while b'\n' in pending:
                    raw, pending = pending.split(b'\n', 1)
                    line = raw.decode('utf-8', 'replace').rstrip('\r')
                    ward = make_ward(line)
                    if ward:
                        sock.sendall(ward.encode('ascii'))
                        print(f'[auto] {ward}', file=sys.stderr, end='', flush=True)
    finally:
        selector.close()
        sock.close()


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError) as exc:
        print(f'Client error: {exc}', file=sys.stderr)
        raise SystemExit(1)
