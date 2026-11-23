# xorcism

**Category** cctf

## Solution

1. I downloaded `demon.c` and inspected its source on my Mac. The program checked a 33-character password against an XOR-encoded byte array using a repeating four-byte key.

   ```bash
   cd ~/Downloads
   ls -l demon.c
   cat demon.c
   ```

2. I found the encoded bytes, key, and validation expression in the program.

   ```c
   #define PWLEN 33
   static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };
   // From the password-checking loop:
   (unsigned char)in[i] != (enc[i] ^ key[i % 4])
   ```

3. XOR is reversible: if `encoded = plaintext ^ key`, then `plaintext = encoded ^ key`. I created `solve.py` to apply that operation to each byte, cycling through the key with `i % 4`.

   ```bash
   nano solve.py
   ```

   ```python
   enc = [
       0x47, 0x4d, 0x1f, 0x6e, 0x64, 0x44, 0x05, 0x4b,
       0x78, 0x6f, 0x2e, 0x50, 0x71, 0x64, 0x3f, 0x4b,
       0x68, 0x55, 0x32, 0x6a, 0x68, 0x4f, 0x3b, 0x5c,
       0x76, 0x4d, 0x05, 0x6b, 0x04, 0x4c, 0x15, 0x41,
       0x44
   ]
   key = [0x37, 0x21, 0x5a, 0x0f]

   password = ''
   for i in range(len(enc)):
       password += chr(enc[i] ^ key[i % 4])

   print('Password:', password)
   print('Flag: cctf{' + password + '}')
   ```

4. I ran the Python script to recover the password, then built the original program and entered the decoded password to verify it.

   ```bash
   python3 solve.py
   clang -o demon demon.c
   ./demon
   ```

5. The decoded password produced the CTF flag.

   ```text
   Password: plEaSe_DONt_FEeD_the_naSAl_d3mONs
   Flag: cctf{plEaSe_DONt_FEeD_the_naSAl_d3mONs}
   ```

## Подробный разбор

В задании `xorcism` мне нужно было восстановить пароль, который студент спрятал в C-программе с помощью XOR. В отличие от `cavern`, здесь не требовался контейнер: я получила `demon.c` и могла изучить, скомпилировать и запустить его локально на Mac.

### 1. Я открыла `demon.c` и изучила проверку пароля

```bash
cd ~/Downloads
ls -l demon.c
cat demon.c
```

В начале файла я увидела константу `PWLEN 33`, массив `enc` из 33 зашифрованных байтов и ключ из четырёх байтов. В `main()` программа читает строку через `fgets()`, удаляет перевод строки и сначала проверяет её длину. Затем сравнивает каждый символ введённого пароля с результатом XOR.

```c
for (int i = 0; i < PWLEN; i++) {
    if ((unsigned char)in[i] != (enc[i] ^ key[i % 4])) {
        puts("nope");
        return 1;
    }
}
```

Я поняла, что перебирать пароль не нужно: в исходнике уже есть всё для обратного преобразования.

### 2. Я разобралась, как работает XOR

В C оператор `^` означает побитовое исключающее ИЛИ — XOR. Если один и тот же ключ применить два раза, исходное значение восстановится: `(A ^ K) ^ K = A`.

Например, для первого байта:

```text
enc[0] = 0x47
key[0] = 0x37
0x47 ^ 0x37 = 0x70 = 'p'
```

Выражение `key[i % 4]` означает, что четырёхбайтовый ключ повторяется по кругу. При `i = 0` используется первый байт ключа, при `i = 4` — снова первый, и так далее.

### 3. Я написала `solve.py` для расшифровки всех байтов

```bash
nano solve.py
```

```python
enc = [
    0x47, 0x4d, 0x1f, 0x6e, 0x64, 0x44, 0x05, 0x4b,
    0x78, 0x6f, 0x2e, 0x50, 0x71, 0x64, 0x3f, 0x4b,
    0x68, 0x55, 0x32, 0x6a, 0x68, 0x4f, 0x3b, 0x5c,
    0x76, 0x4d, 0x05, 0x6b, 0x04, 0x4c, 0x15, 0x41,
    0x44
]
key = [0x37, 0x21, 0x5a, 0x0f]

password = ''
for i in range(len(enc)):
    password += chr(enc[i] ^ key[i % 4])

print('Password:', password)
print('Flag: cctf{' + password + '}')
```

В цикле `enc[i] ^ key[i % 4]` восстанавливает ASCII-код очередного символа. `chr()` переводит числовой код в символ, а `password +=` собирает символы в строку. После сохранения файла я запустила его с помощью Python 3.

### 4. Я получила пароль и проверила его в исходной C-программе

```bash
python3 solve.py
clang -o demon demon.c
./demon
```

Скрипт вывел:

```text
Password: plEaSe_DONt_FEeD_the_naSAl_d3mONs
Flag: cctf{plEaSe_DONt_FEeD_the_naSAl_d3mONs}
```

Я скомпилировала оригинальный `demon.c`, чтобы проверить расшифровку независимо от Python-скрипта. При запуске `./demon` программа запрашивает `password:`; туда я ввела найденную строку. Оригинальная программа формирует флаг, добавляя к правильному паролю обрамление `cctf{...}`.

### 5. Я отправила восстановленный флаг

В этом задании я не использовала подбор паролей или эксплуатацию памяти. Достаточно было понять симметричность XOR и повторение ключа из четырёх байтов. Полученный флаг был принят ботом и учтён в итоговом счёте CTF.
