# needle

Flag: threadctf{sHo0tIng_50M3_4T0Mic_FEet}
Instance: threadctf (findingnemo).

Finding: the meeting program reads the flag in a thread while it is still root,
THEN drops to the normal user and loads my code. Dropping root does not erase the
flag already in memory, and my code runs in the same process. I pthread_join the
program's own thread to get the flag pointer back and print it.

Locations:
- Host: osctf.nnk.moe, container: ssh m.torebaev-needle@osctf.nnk.moe
  (password from the bot). VPN off, or ssh -b <my Ethernet IP>.
- Binary: ~/meeting, setuid root (-rwsr-xr-x root root). That is why it can read
  /root/flag.txt. After it drops root I cannot: cat /root/flag.txt -> Permission denied.
- Source Meeting.c: thread retrieve_the_fabric() reads /root/flag.txt into malloc(256)
  and returns that pointer; main then setgid/setuid down and dlopen("./libmeeting.so")
  and runs my meeting().
- The flag pointer is the return value of the global thread handle
  pthread_t retrieval. The binary is EXEC (not PIE) and not stripped, so that global
  is at a fixed address I read with nm.
- My code: program_with_name.c -> libmeeting.so. In meeting() I read the retrieval
  handle from that address, pthread_join it, and write the flag to stdout, _exit(0)
  (so I never hand over a "name").

Commands:
  readelf -h ~/meeting | grep Type          # Type: EXEC  -> fixed addresses
  nm ~/meeting | grep retrieval             # 0000000000404128 B retrieval
  scp program_with_name.c m.torebaev-needle@osctf.nnk.moe:/home/ctf/program_with_name.c
  cd ~
  gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread
  ./meeting

Note: re-run nm each round, the retrieval address is not always 0x404128.

Output:
  threadctf{sHo0tIng_50M3_4T0Mic_FEet}
