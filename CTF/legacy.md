# legacy

**Category** cctf — Portability / integer width

## Solution

1. I inspected `legacy.c` and the diagnostics from the original developer: `unsigned` was 18 bits and the check value was `237614`.

2. I found that `position()` computes a rolling hash using `unsigned`, while `add_data()` uses the hash as an array index. On my Mac, `unsigned` is 32 bits, so the arithmetic did not match the original machine.

3. I edited `position()` to retain only the low 18 bits after each step:

   ```c
   unsigned position(const char *s)
   {
       unsigned h = 0;
       while (*s) {
           h = (h * 67 + *s++) & 0x3FFFF;
       }
       return h;
   }
   ```

4. I compiled and ran the fixed source locally on macOS.

   ```bash
   cd ~/Downloads
   clang -o legacy_fixed legacy.c
   ./legacy_fixed
   ```

5. The check value matched the reference, and the decrypted document contained the flag.

   ```text
   unsigned has 32 bits
   check value: 237614
   cctf{thE_G0oD_oLd_Days_wiTh_90Od_old_c}
   ```

## Подробный разбор

Для `legacy` отдельный SSH-контейнер не требовался: я работала с `legacy.c` локально на Mac. Программа падала, потому что функция `position()` рассчитывала индекс в массиве с помощью `unsigned`, а на машине первоначального разработчика этот тип имел 18 бит. На моём Mac он имеет 32 бита.

Я добавила маску `0x3FFFF` (то есть `2^18 - 1`) к вычислению хеша. Это имитирует переполнение 18-битного беззнакового целого. После компиляции `clang` диагностическое значение стало `237614`, как в условии, а программа вывела восстановленный документ с флагом.
