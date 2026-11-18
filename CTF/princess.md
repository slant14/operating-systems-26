# princess

**Category** threadctf

## Solution

1. I investigated the running processes rather than searching for a regular file containing the flag. The challenge represented princesses as zombie processes and cabins as their parent processes.

   ```bash
   ps -eo pid,ppid,stat,comm,args | grep -E 'princess|cabin'
   ```

2. I identified zombie princesses by the `Z` process state and used their parent process IDs (`PPID`) to find the corresponding cabins.

   ```bash
   ps -eo pid,ppid,stat,comm,args
   ```

3. I terminated the relevant cabin processes as required by the challenge. The successful run was automated with Bash, using the process list to locate the parents of zombie princesses.

4. After the challenge condition was met, I read the resulting flag file.

   ```bash
   cat /home/ctf/flag.txt
   ```

   ```text
   threadctf{WHaT_Do_YoU_MeaN_7hey_4Re_41re4dY_Dead}
   ```

## Подробный разбор

### 1. Я проверила процессы

В задании `princess` принцессы были представлены процессами Linux. Ключевая особенность — они уже находились в состоянии **zombie** (`Z`), то есть завершили выполнение, но родитель ещё не забрал их статус через `wait()`.

Команда `ps` позволила увидеть PID, PPID и состояние процессов:

```bash
ps -eo pid,ppid,stat,comm,args
```

### 2. Я связала принцесс с домиками

Для каждого процесса-принцессы в состоянии `Z` я определила `PPID`. Этот номер указывал на родительский процесс — соответствующий `cabin`. Таким образом, нужно было работать не с уже завершёнными zombie-процессами, а с их родителями.

### 3. Я выполнила условие задания

Я автоматизировала поиск нужных родительских процессов в Bash и завершила соответствующие `cabin`. После выполнения условия задания появился доступ к флагу.

### 4. Я получила флаг

```bash
cat /home/ctf/flag.txt
```

```text
threadctf{WHaT_Do_YoU_MeaN_7hey_4Re_41re4dY_Dead}
```

**Главная идея:** zombie-процесс уже завершён; искать нужно его родителя по `PPID`.
