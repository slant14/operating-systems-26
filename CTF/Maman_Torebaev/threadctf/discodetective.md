# discodetective

Flag: threadctf{PU7_0N_PaNt5_Not_ImpoS7eR_sYNDr0ME}
Instance: threadctf (findingnemo).

Finding: the game is a server with a 3-second answer window, too fast for a human.
I wrote a client with two threads: one answers the "intrusive thought" strikes
within the window, the other plays the case (asks questions, accuses, proves).

Locations:
- Host: osctf.nnk.moe, container: ssh m.torebaev-discodetective@osctf.nnk.moe
  (password from the bot). VPN must be off, or ssh -b <my Ethernet IP>.
- The game: python3 /root/discodetective.py, runs as root (seen with ps aux).
  I cannot read its code, so I only talk to it over the socket.
- It listens on 127.0.0.1:9999 (localhost, inside the container only).
- The protocol: every machine line starts with !! and has key=value fields
  (!!THOUGHT-STRIKE sigil=.. verb=.. form=.. action=.., !!FACT kind=.., !!PICK, ...).
  I read those, not the story text.
- My client: solve.py. A reader thread answers strikes (WARD <sigil> <verb>
  <action in kebab/reversed/shout>, twice for SPLIT); a main thread picks
  questions to complete two "strands" and then ACCUSE + PROOF. A threading.Lock
  stops the two threads from writing on the socket at the same time.

Commands:
  scp solve.py m.torebaev-discodetective@osctf.nnk.moe:/tmp/solve.py
  ssh m.torebaev-discodetective@osctf.nnk.moe
  python3 /tmp/solve.py

Output (each game is random, ~1 in 3 wins):
  == game 2: FLAG threadctf{PU7_0N_PaNt5_Not_ImpoS7eR_sYNDr0ME}
