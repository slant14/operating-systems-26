# xorcism

**Category** cctf — Reverse engineering / XOR

## Solution

1. I opened `demon.c` and found the 33-byte `enc` array, the four-byte `key`, and the comparison `in[i] == (enc[i] ^ key[i % 4])`.

   ```bash
   cd ~/Downloads
   cat demon.c
   ```

2. Because XOR with the same key is reversible, I wrote `solve.py` to decode the password.

   ```python
   enc = [
       0x47, 0x4d, 0x1f, 0x6e, 0x64, 0x44, 0x05, 0x4b,
       0x78, 0x6f, 0x2e, 0x50, 0x71, 0x64, 0x3f, 0x4b,
       0x68, 0x55, 0x32, 0x6a, 0x68, 0x4f, 0x3b, 0x5c,
       0x76, 0x4d, 0x05, 0x6b, 0x04, 0x4c, 0x15, 0x41, 0x44
   ]
   key = [0x37, 0x21, 0x5a, 0x0f]
   password = ''.join(chr(x ^ key[i % 4]) for i, x in enumerate(enc))
   print('Password:', password)
   print('Flag: cctf{' + password + '}')
   ```

3. I ran the script and compiled the original C program to verify the recovered password.

   ```bash
   python3 solve.py
   clang -o demon demon.c
   ./demon
   ```

4. I entered the decoded password when prompted and recovered the flag.

   ```text
   Password: plEaSe_DONt_FEeD_the_naSAl_d3mONs
   cctf{plEaSe_DONt_FEeD_the_naSAl_d3mONs}
   ```

## Подробный разбор

В `demon.c` я нашла зашифрованный массив `enc` длиной 33 байта и четырёхбайтовый ключ `key`. Программа сравнивала введённый пароль с результатом XOR: `enc[i] ^ key[i % 4]`. Так как XOR обратим, пароль можно получить непосредственно из этих массивов, не перебирая варианты.

Я создала `solve.py`, в цикле выполнила XOR для каждого байта и преобразовала результаты в символы функцией `chr()`. После запуска скрипта я получила пароль. Затем скомпилировала `demon.c` командой `clang -o demon demon.c` и проверила пароль в исходной программе.
