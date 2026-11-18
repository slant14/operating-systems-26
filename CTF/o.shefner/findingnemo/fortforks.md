# CTF `fortforks` — report

**Instance:** `findingnemo` (`threadctf`).
**Challenge:** `fortforks`.

## Observations

After connecting over SSH, the working directory is `/home/ctf`. Commands:

```bash
pwd
ls -la /home/ctf
ps -eo pid,ppid,etime,comm,args | head -35
```

`/home/ctf/secrets` is a FIFO (`p` at the start of the `ls -l` output), rather than a regular file. The container runs `python3 /root/forks.py` and `python3 /root/limited_killer.py`. The SSH session is terminated after about 5 seconds. Reopening the FIFO restarts reading from the beginning.

## Solution

The [`fortforks.py`](fortforks.py) program opens the FIFO once with `O_NONBLOCK`, detaches from the SSH session using `fork` and `setsid`, reads the stream, and creates a new child process every 1.5 seconds. The old process exits. The new process inherits **the same open FIFO file descriptor**, so reading continues from the previous position. The program keeps the last 128 KiB of the stream in memory and saves them at EOF.

After transferring the file to the container:

```bash
python3 /home/ctf/fortforks.py
```

To verify the result:

```bash
tail -n 10 /home/ctf/fortforks.log
ls -l /home/ctf/fortforks.result
tail -c 2000 /home/ctf/fortforks.result
```

The log confirmed `EOF: read 2097243 bytes; saved final 131072 bytes to /home/ctf/fortforks.result`. The last line of the result:

```text
Congratulations, Agent. The flag is: threadctf{0NLy_pROper_LINEag3_get_tO_l1VE_iN_cAs71e}
```

**Result location:** `/home/ctf/fortforks.result`.
