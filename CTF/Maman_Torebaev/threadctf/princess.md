# princess

Flag: threadctf{wH4t_DO_YOU_MEan_7H3y_Are_a1R3aDY_DeAD}
Instance: threadctf (findingnemo).

Finding: a "cursed cabin" is a process whose only princess child is a zombie
(STAT Z) the cabin never reaps. You cannot kill a zombie. But if I kill the cabin,
its zombie becomes an orphan, PID 1 adopts it and reaps it. After I tear down 50
cursed cabins the forest (PID 1) writes the flag. Living princesses must stay.

Locations:
- Host: osctf.nnk.moe, container: ssh m.torebaev-princess@osctf.nnk.moe
  (password from the bot). VPN off if ssh hangs on banner exchange.
- PID 1 = the_forest (/root/forest), parent of every cabin, and what writes the flag.
- A cabin = a child of PID 1, program /usr/local/bin/cabin.
- A princess = a child of a cabin, named princess_*. STAT Z (<defunct>) = dead =
  the cabin is cursed; STAT S = alive = do not touch that cabin.
- Flag file: ~/flag.txt (/home/ctf/flag.txt), owner root, appears after 50 cursed
  cabins are killed.
- My loop: solve.sh (shell) / kill50.py (python): every ~1s find children of PID 1
  that run /usr/local/bin/cabin with exactly one princess_ child in state Z, and
  SIGTERM the cabin (not the princess).

Commands:
  ssh m.torebaev-princess@osctf.nnk.moe
  ps -eo pid,ppid,stat,comm --forest                     # see cabins under PID 1
  ps -eo pid,ppid,stat,args | grep defunct | grep -v grep # zombies; col 2 (PPID) = cursed cabin
  kill <cabin PID>                                        # SIGTERM the cabin, orphan -> PID 1 reaps
  bash solve.sh                                           # loop until ~/flag.txt appears
  cat ~/flag.txt

Output:
  42: cabin 1985 torn down
  threadctf{wH4t_DO_YOU_MEan_7H3y_Are_a1R3aDY_DeAD}
