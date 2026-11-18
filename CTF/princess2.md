# princess2

**Category** threadctf

## Solution

1. I worked in the challenge's restricted shell, where the process-based puzzle had to be solved using a C program.

2. I inspected `/proc` to identify zombie princess processes. For each candidate, I checked its state and parent PID, then inspected the parent's command line to confirm that it was a cabin.

   ```text
   /proc/<pid>/stat
   /proc/<parent-pid>/cmdline
   ```

3. I used `kill(parent_pid, SIGTERM)` from C to terminate the matching cabin processes, repeating the operation until the challenge's target of 100 cabins was reached.

   ```c
   /* Core operation after verifying a zombie princess and its cabin parent. */
   kill(parent_pid, SIGTERM);
   ```

4. The successful run returned the flag.

   ```text
   threadctf{1_h0pE_You_CaN_C_C134rIY_NOw}
   ```

## Подробный разбор

### 1. Я изучила ограничения

В `princess2` задача была похожа на `princess`, но окружение ограничивало привычные способы просмотра процессов. Поэтому я использовала программу на C и виртуальную файловую систему `/proc`.

### 2. Я нашла zombie-процессы

В Linux `/proc/<pid>/stat` содержит состояние процесса и PID его родителя. Я отбирала процессы-принцессы в состоянии `Z` и находила их родителей.

### 3. Я проверила родителей

Чтобы не завершать посторонние процессы, я дополнительно проверяла `/proc/<ppid>/cmdline`: родитель должен был соответствовать `cabin`.

### 4. Я завершила нужные домики

В C вызов `kill(pid, SIGTERM)` отправляет процессу сигнал завершения. Я повторяла поиск и завершение для требуемого количества домиков — 100.

### 5. Я получила флаг

```text
threadctf{1_h0pE_You_CaN_C_C134rIY_NOw}
```

**Главная идея:** `/proc` позволяет получить сведения о процессах непосредственно из ядра даже без удобных команд вроде `ps`.
