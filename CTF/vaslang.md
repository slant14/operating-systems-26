# vaslang

**Category** cctf — Interpreter debugging / forward references

## Solution

1. I downloaded `vaslang.c` and `inp.vas` to `~/Downloads`, compiled the original interpreter, and ran the supplied input.

   ```bash
   cd ~/Downloads
   clang -o vaslang vaslang.c
   ./vaslang < inp.vas > output.txt
   head -n 15 output.txt
   tail -n 3 inp.vas
   ```

2. The output contained errors such as `ERROR: func hopeful_893 undefined`. The input ended with `main!`, and many function bodies referenced functions defined later.

3. I backed up the source and created a Python patch script (`fix.py`) to modify the interpreter.

   ```bash
   cp vaslang.c vaslang_original.c
   nano fix.py
   python3 fix.py
   ```

4. The patch deferred resolving named function calls until execution, storing references to call terms and function names. Before executing a command, `resolve_calls()` looked up each final function body and populated the call's `target`. I also changed `xdef()` to increment `funcp` only when adding a new function, not when redefining one.

5. I compiled and executed the patched interpreter and extracted the flag from its output.

   ```bash
   clang -O2 -o vaslang_fixed vaslang.c
   ./vaslang_fixed < inp.vas > output_fixed.txt
   tail -n 10 output_fixed.txt
   grep -o 'cctf{[^}]*}' output_fixed.txt
   ```

6. The `grep` output was:

   ```text
   cctf{you_CouLd_HaVE_SO1VEd_tHi5_Th3_fUn_waY}
   ```

## Подробный разбор

Я скачала `vaslang.c` и большой файл `inp.vas` (около 1,1 МБ). Оригинальный интерпретатор компилировался, но при обработке входа выдавал множество ошибок `func ... undefined`. Причина заключалась в том, что имена функций разрешались сразу при разборе определения, хотя нужные функции могли объявляться ниже.

Я сделала резервную копию `vaslang.c` и с помощью `fix.py` изменила обработку вызовов: теперь интерпретатор сначала сохранял ссылки на вызываемые функции, а перед выполнением разрешал их по готовой таблице определений. Дополнительно исправила увеличение `funcp` при переопределении существующей функции. После повторной компиляции интерпретатор выполнил `inp.vas`. Вывод содержал много символов `>` (приглашений командной строки), поэтому я извлекла флаг регулярным выражением через `grep -o`.
