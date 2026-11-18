# 03SpyDog: Recovering an unreachable Git object

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The `cat_tracker` repository retains an image in a commit that is no longer reachable from ordinary branch history.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/dog/Doguments/cat_tracker
git fsck --no-reflogs --unreachable
git ls-tree -r 4936471
git show 4936471:cat_cam/frame_0044.png > /tmp/Spy_dog.png
file /tmp/Spy_dog.png
```

The saved run identified commit `4936471` and extracted `cat_cam/frame_0044.png`. Both identifiers are specific to that repository instance. `git show` writes the original blob bytes without checking out an old branch.

## Result

Recovered artifact: [Spy_dog.png](../found/Spy_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Removing a commit from visible history does not immediately remove its objects.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
