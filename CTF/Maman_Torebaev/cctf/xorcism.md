# xorcism

Flag: cctf{Ple4se_d0nT_f3eD_th3_NasAl_demOns}

Finding: the password is stored XOR-encrypted, but the key is right next to it in
the source. XOR is its own inverse, so I get the password back directly. No brute
force, no need to run the binary.

Locations:
- No ssh container for this one: both arrays are in the given source (demon.c), so it is solved locally, no server needed.
- Program: demon.c, a password checker.
- Ciphertext: static const unsigned char enc[33] in demon.c (password XOR key).
- Key: static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f }; (stored right
  beside enc[]).
- Check: it accepts input only if in[i] == enc[i] ^ key[i % 4], then prints
  "correct! flag: cctf{%s}".
- Recovery: pw[i] = enc[i] ^ key[i % 4]  -> solve.py.

Commands:
  python3 solve.py

Output:
  password: Ple4se_d0nT_f3eD_th3_NasAl_demOns
  flag:     cctf{Ple4se_d0nT_f3eD_th3_NasAl_demOns}

Check against the real binary:
  gcc -o demon demon.c
  printf '%s\n' 'Ple4se_d0nT_f3eD_th3_NasAl_demOns' | ./demon
  -> password: correct! flag: cctf{Ple4se_d0nT_f3eD_th3_NasAl_demOns}
