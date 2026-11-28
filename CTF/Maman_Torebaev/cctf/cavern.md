# cavern

Flag: cctf{ReMeMber_7H3Re_is_no_SPoOn_frEe_frOM_na5AL_DeM0Ns}

Finding: use-after-free. cavern loads a .so I give it and calls my run(). It
decodes the flag into a heap struct, frees it, but hands back the same pointer.
The freed memory still holds the flag, so I just read it back.

Locations:
- Host: osctf.nnk.moe, container: ssh m.torebaev-cavern@osctf.nnk.moe
- Binary: ~/cavern, setuid root (-rws--x--x root root). I can run it but not read it,
  so the flag can only be taken from the live root process (my run() has euid=0).
- Bug: function guess() in cavern.c. It calloc's struct offering { char guess[64];
  char secret[64]; }, decodes the flag into secret, then free()s the chunk and
  returns the dangling pointer.
- The flag lives in the field secret, at offset 64 in the chunk. free() only writes
  the first 16 bytes (tcache header, in guess[]), so secret survives.
- The flag on disk is XOR-encoded in a static array fenc[55] (key deadbeef), never
  decoded to a file.
- My exploit: /tmp/offer.c -> /tmp/offer.so. It finds guess() with
  dlsym(RTLD_DEFAULT, "guess") and reads o->secret right after the call.

Commands:
  ssh m.torebaev-cavern@osctf.nnk.moe
  cd /tmp
  gcc -shared -fPIC -o offer.so offer.c -ldl
  ~/cavern /tmp/offer.so

Output:
  [*] run() euid=0 uid=1000
  [+] FLAG string : cctf{ReMeMber_7H3Re_is_no_SPoOn_frEe_frOM_na5AL_DeM0Ns}
