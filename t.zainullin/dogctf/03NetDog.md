# dogctf — 03NetDog (100 points)

**Flag:** picture file (300x222 PNG streamed over TCP).

## Task

The container runs `/usr/local/bin/netdog` (as root, PID 1 child) which
prints `Using port: 1540` when invoked.

## Solution

netdog listens on TCP port 1540 and streams a PNG to anyone who connects.
Pure bash, no `nc` needed:

```sh
timeout 15 bash -c 'exec 3<>/dev/tcp/127.0.0.1/1540; cat <&3 > /tmp/netdog_flag.png'
file /tmp/netdog_flag.png
# PNG image data, 300 x 222, 8-bit/color RGBA, non-interlaced
```

Send `Net_dog.png.dat` to the bot as a document.

## Result

```
✅ Flag accepted for '03NetDog'.
```
