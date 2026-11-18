# cctf - xorcism

**Flag:** `cctf{p1eaS3_Dont_FeeD_tHE_na5Al_D3mOns}`

## Task

The bot gives a file `demon.c`. It contains a 33-byte array `enc` and a 4-byte
key. The program asks for a password, XORs every input byte with
`key[i % 4]` and compares with `enc`. If all 33 bytes match, it prints:

```c
printf("correct! flag: cctf{%s}\n", in);
```

So the password **is** the flag content.

## Idea

XOR is symmetric: `enc[i] ^ key[i % 4]` gives the original password bytes.



Recover the password with Python:

```python
enc = bytes.fromhex('47103f6e6412054b584f2e5071443f4b'
                    '6855124a684f3b3a764d054b044c156144')
key = bytes.fromhex('37215a0f')
pw = ''.join(chr(enc[i] ^ key[i % 4]) for i in range(len(enc)))
print(pw)   # p1eaS3_Dont_FeeD_tHE_na5Al_D3mOns
```

Verify with the real program:

```
gcc -o demon demon.c
printf 'p1eaS3_Dont_FeeD_tHE_na5Al_D3mOns\n' | ./demon
```

Output:

```
crack me!
password: correct! flag: cctf{p1eaS3_Dont_FeeD_tHE_na5Al_D3mOns}
```
