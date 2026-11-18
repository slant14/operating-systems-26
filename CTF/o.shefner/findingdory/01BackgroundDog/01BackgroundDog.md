# Bash CTF: `01BackgroundDog`

**Instance:** `findingdory` (`dogctf`).
**Result:** the bot replied `✅ Flag accepted for '01BackgroundDog'.`

After `/challenge 01BackgroundDog`, we connected to the container over SSH. On login, the shell displayed a stopped background job:

```text
[1]+ Stopped /usr/local/bin/background_dog /tmp/bgdog
```

Checking the state:

```bash
jobs -l
ls -la /tmp/bgdog
ps -ef | grep '[b]ackground_dog'
```

The `jobs -l` command showed the stopped `/usr/local/bin/background_dog` process; `/tmp/bgdog` did not exist. We resumed the job in the foreground:

```bash
fg %1
```

The program printed the image path: `/regular_paw_storage/ecd6c7b966d6f4f6/Background_dog.png`. We checked the file:

```bash
ls -l /regular_paw_storage/ecd6c7b966d6f4f6/Background_dog.png
file /regular_paw_storage/ecd6c7b966d6f4f6/Background_dog.png
```

The check identified a PNG, 300 × 150, 123251 bytes. From **the local Ubuntu system**, outside the SSH session, we downloaded the file:

```bash
scp o.shefner-dog@osctf.nnk.moe:/regular_paw_storage/ecd6c7b966d6f4f6/Background_dog.png .
```

We sent the image to the bot as a file, and it confirmed that the flag was accepted. A copy of the PNG is saved alongside this report as [`Background_dog.png`](Background_dog.png).
