# dogctf - 01BackgroundDog

**Flag:** picture file (`Background_dog.png`, 300x150 PNG).

## Task

`.bash_profile` of the container starts a background job:

```
set -m
echo "suppa_seeccreet_kiy" > /tmp/bgdog
/usr/local/bin/background_dog /tmp/bgdog &
```



`background_dog` watches `/tmp/bgdog`, consumes the key (the file disappears)
and then **repeatedly prints the flag path** into the session it was started
from. That session's output was captured in `/tmp/check.log`:

```
/regular_paw_storage/b34a5950a15e0db7/Background_dog.png   (repeated many times)
```

(The `/regular_paw_storage/<12hex>/` dirs are `drwx--x--x` root-owned: no
listing, but direct access by full path works.)

```sh
cp /regular_paw_storage/b34a5950a15e0db7/Background_dog.png /tmp/
```

