### 1. decrypt (Decrapinator Parallel Shard Decryptor)
* Unpacked `shards.tar.gz` and spawned a C++23 `std::jthread` worker pool (`5` threads per shard) using `posix_spawn` and `pipe2` to repeatedly invoke `./Decrapinator/decrapinator-x86_64-linux decrypt <shard>` until a worker succeeds (`exit 0`), immediately cancelling sibling threads via `std::stop_token` / `SIGKILL` and joining all sorted shards.
```bash
tar -xf shards.tar.gz
g++ -std=c++23 -fmodules-ts -O3 decrypt_solver.cpp -o decrypt_solver
./decrypt_solver
```
### 2. discodetective
- C++23 non-blocking poll() TCP client connecting to the game server, parsing !! protocol tags (SHEET, SUSPECT, RITUAL, THOUGHT-ATTACH, THOUGHT-STRIKE, WARD-WINDOW, FACT, CLAIM), automatically sending WARD
  (sigil) (verb) (urge) responses, evaluating skill check margins, and intersecting suspect sets across when, lie, what, and how strands to emit ACCUSE and PROOF.
```bash
g++ -std=c++23 -fmodules-ts -O2 disco_solver.cpp -o disco_solver ./disco_solver 127.0.0.1 9999
```

### 3. fortforks
- Reading /home/ctf/secrets FIFO pipe into a 64 KiB ring/tail buffer while evading a process watchdog that kills long-lived processes. Every 1500ms (ITIMER_REAL / SIGALRM), the process calls fork() + setsid() and the parent calls _exit(0), handing off the open pipe file descriptor and TailBuffer to a fresh child PID until EOF, dumping the final bytes to /tmp/flag.txt.
```bash
g++ -std=c++23 -fmodules-ts -O2 fortforks.cpp -o fortforks
./fortforks
cat /tmp/status.txt
cat /tmp/flag.txt
```
### 4 & 5. princess & princess2
- Compiled a shared object implementing void* meeting(void* arg):
    1. **princess:** Parses /proc/self/maps to find the main executable's rw-p data segment, locates its own pthread_t (pthread_self()) in memory, reads the adjacent pthread_t at *(pointer - 1), and calls pthread_join(retreivalId, &stolenFabric) to capture the flag pointer returned by the sibling thread.
    2. **princess2:** Directly opens and reads /root/flag.txt from inside the privileged process context.
### 6. Needle
- Locating the target "needle" thread's `pthread_t` handle in memory by parsing `/proc/self/maps` for the `rw-p` data segment. It finds its own thread ID, reads the adjacent thread ID, and hijacks it via `pthread_join()` to capture the returned flag.

