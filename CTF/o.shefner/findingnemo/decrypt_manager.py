#!/usr/bin/env python3
"""Run an unreliable decryptor concurrently; keep only successful stdout.

Example (ONLY if the decryptor accepts the shard path as its argument):
  python3 decrypt_manager.py shards -- ./decrapinator-x86_64-linux '{shard}'
For a decryptor reading stdin, add --stdin before -- and omit {shard}.
"""
import argparse
import asyncio
from pathlib import Path
import sys
import time


async def attempt(command, shard, via_stdin, slots):
    async with slots:
        proc = await asyncio.create_subprocess_exec(
            *(arg.replace('{shard}', str(shard)) for arg in command),
            stdin=asyncio.subprocess.PIPE if via_stdin else asyncio.subprocess.DEVNULL,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.DEVNULL,
        )
        # Keep communicate alive on cancellation so pipes are drained and the
        # child is reaped even if the global deadline expires.
        reader = asyncio.create_task(proc.communicate(shard.read_bytes() if via_stdin else None))
        try:
            stdout, _ = await asyncio.shield(reader)
            return proc.returncode, stdout
        finally:
            if proc.returncode is None:
                try:
                    proc.kill()
                except ProcessLookupError:
                    pass
            await reader


async def solve_one(shard, args, slots, results, counts):
    async def retry_loop():
        for _ in range(args.attempts):
            counts[shard.stem] += 1
            code, output = await attempt(args.command, shard, args.stdin, slots)
            if code == 0:
                return output  # Preserve all bytes, including spaces/newlines.
            # A crash can leave partial stdout. Discard ALL of that output.
            await asyncio.sleep(0.01)
        raise RuntimeError(f'{shard.name}: retry limit reached')

    replicas = [asyncio.create_task(retry_loop()) for _ in range(args.copies)]
    try:
        failures = []
        for finished in asyncio.as_completed(replicas):
            try:
                output = await finished
            except RuntimeError as exc:
                failures.append(str(exc))
                continue
            results[int(shard.stem)] = output
            print(f'[{len(results)}/{args.total}] {shard.name}', file=sys.stderr)
            return
        raise RuntimeError('; '.join(failures))
    finally:
        for task in replicas:
            if not task.done():
                task.cancel()
        await asyncio.gather(*replicas, return_exceptions=True)


async def run(args, shards):
    results = {}
    counts = {p.stem: 0 for p in shards}
    slots = asyncio.Semaphore(args.parallel)
    started = time.monotonic()
    tasks = [asyncio.create_task(solve_one(p, args, slots, results, counts)) for p in shards]
    success = False
    try:
        await asyncio.wait_for(asyncio.gather(*tasks), timeout=args.seconds)
        success = True
    except (asyncio.TimeoutError, RuntimeError, OSError) as exc:
        print(f'FAILED: {type(exc).__name__}: {exc}', file=sys.stderr)
    finally:
        for task in tasks:
            if not task.done():
                task.cancel()
        await asyncio.gather(*tasks, return_exceptions=True)
    elapsed = time.monotonic() - started
    print(f'{len(results)}/{len(shards)} shards; {sum(counts.values())} attempts; {elapsed:.2f}s', file=sys.stderr)
    if not success:
        missing = [p.name for p in shards if int(p.stem) not in results]
        print('Missing: ' + ', '.join(missing), file=sys.stderr)
        print('No flag file written. Any pre-existing output file is unchanged.', file=sys.stderr)
        return 1
    flag = b''.join(results[int(p.stem)] for p in shards)
    # Exclusive creation prevents accidentally presenting an old flag as new.
    with args.output.open('xb') as stream:
        stream.write(flag)
    print(f'Saved {len(flag)} bytes to {args.output}', file=sys.stderr)
    sys.stdout.buffer.write(flag)
    sys.stdout.buffer.flush()
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('shards', type=Path)
    parser.add_argument('--output', type=Path, default=Path('flag.txt'))
    parser.add_argument('--seconds', type=float, default=25,
                        help='Local execution budget only; excludes download/submission')
    parser.add_argument('--parallel', type=int, default=256, help='Maximum concurrent child processes')
    parser.add_argument('--copies', type=int, default=2, help='Concurrent attempts per shard; first success wins')
    parser.add_argument('--attempts', type=int, default=100, help='Maximum attempts per copy')
    parser.add_argument('--stdin', action='store_true', help='Pass shard bytes on standard input')
    # Explicit -- separates manager arguments from the arbitrary decryptor command.
    argv = sys.argv[1:]
    if '--' not in argv:
        parser.error("use: SHARDS [options] -- DECRYPTOR [arguments containing {shard}]")
    split = argv.index('--')
    args = parser.parse_args(argv[:split])
    args.command = argv[split + 1:]
    if not args.command or (not args.stdin and not any('{shard}' in a for a in args.command)):
        parser.error('provide a decryptor command with {shard}, or use --stdin')
    if min(args.seconds, args.parallel, args.copies, args.attempts) <= 0:
        parser.error('numeric limits must be positive')
    if args.output.exists():
        parser.error(f'output already exists: {args.output}; choose a new --output')
    shards = list(args.shards.glob('*.shard'))
    if not shards or any(not p.stem.isascii() or not p.stem.isdigit() for p in shards):
        parser.error('expected numeric filenames such as 0.shard, 1.shard, ...')
    shards.sort(key=lambda p: int(p.stem))
    if [int(p.stem) for p in shards] != list(range(len(shards))):
        parser.error('shard numbers must be consecutive from 0, without duplicates')
    args.total = len(shards)
    try:
        return asyncio.run(run(args, shards))
    except KeyboardInterrupt:
        print('Interrupted.', file=sys.stderr)
        return 130


if __name__ == '__main__':
    raise SystemExit(main())
