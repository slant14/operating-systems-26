# 03ProDog: Recovering the short-lived process reward

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

This challenge involves a transient dog process. The personal record preserves the reward announcement and recovered file, but not a definitive sequence that triggered the announcement.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
ps -eo pid,ppid,stat,comm,args
# Recorded reward path:
file /regular_paw_storage/aac1e8451a14e8af/Pro_dog.png
```

On login the container printed `Woof woof! You caught the ProDog! Your bone is buried at:` followed by `/regular_paw_storage/aac1e8451a14e8af/Pro_dog.png`. The file was downloaded over SFTP. Peer reports describe repeatedly inspecting the process list and signalling the short-lived `prodog` process. That explains the mechanism, but is not presented as a recovered personal command log.

## Result

Recovered artifact: [Pro_dog.png](../found/Pro_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Transient processes can disappear between observations; distinguish a confirmed reward from an undocumented trigger.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.

Additional comparison: [PR #34](https://github.com/slant14/operating-systems-26/pull/34), [PR #77](https://github.com/slant14/operating-systems-26/pull/77), [PR #79](https://github.com/slant14/operating-systems-26/pull/79).
