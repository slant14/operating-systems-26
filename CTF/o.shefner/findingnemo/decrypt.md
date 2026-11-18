# decrypt: coordinating unreliable worker processes

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingnemo` (`threadctf`)

## Task

The bot describes an unreliable decryptor that takes about ten seconds per shard and may crash. The complete download, reconstruction and submission must fit a 30-second window. Successful runs write a fragment to stdout. The challenge text says 500 shards, but the archive saved during my earlier work contains **50**, numbered 0 through 49. The manager discovers the actual files rather than hard-coding the description's count.

## Solution design

The [previously prepared manager](decrypt_manager.py) uses `asyncio` to supervise external processes. Independent shards run concurrently. Each shard can have multiple attempts in flight; the first process that exits successfully supplies that shard's bytes. Failed attempts are discarded, including partial stdout, and retried. Other attempts for a completed shard are cancelled and reaped.

Results are stored by numeric shard index and joined only after every shard succeeds. This avoids both completion-order corruption and lexical ordering such as `1, 10, 11, 2`. The manager preserves every output byte, including spaces and newlines, and creates the result exclusively so an old flag cannot be mistaken for a new result.

## Reproduction

Obtain the real decryptor and read its README before running it in a compatible Linux or macOS environment. Select the binary for that architecture. Peer reports use the `decrypt <shard>` command interface:

```bash
tar -xzf shards.tar.gz
chmod +x decrapinator-x86_64-linux
python3 decrypt_manager.py shards --seconds 25 --parallel 256 --copies 2 \
  --output flag.txt -- ./decrapinator-x86_64-linux decrypt '{shard}'
```

This is a reproduction command based on the referenced interface, not a command verified against my saved executable. The earlier personal session did not obtain the actual decryptor. Its invocation and success exit code must therefore be confirmed from the downloaded distribution.

The process limit and number of replicas need to fit available resources. Two replicas are a starting configuration, not a probability guarantee. A CPU-bound ten-second job also depends on core capacity; concurrency does not remove that constraint. The local 25-second timer excludes download and submission and does not control the bot's clock. The export explicitly records an expired attempt and the bot's instruction to start a new attempt with `/challenge decrypt`.

## Result and lessons



The deliverable is the manager and a reproducible strategy. No actual decrypted flag or successful challenge timing is claimed. Unlike examples that accept any nonempty output, this manager checks the exit code: a crashing worker can print an incomplete fragment before failing.

## Sources

Personal saved archive inspection and manager; Telegram prompt `506377` on 5 November 2026; comparison of [PR #34](https://github.com/slant14/operating-systems-26/pull/34), [PR #58](https://github.com/slant14/operating-systems-26/pull/58), [PR #74](https://github.com/slant14/operating-systems-26/pull/74), [PR #80](https://github.com/slant14/operating-systems-26/pull/80). These four PRs support concurrency, retries and ordered assembly, but their process counts and flags are not copied as personal results.
