# needle

**Category** threadctf

## Solution

1. I inspected the `meeting` executable and found the exported symbol `retrieval`.

   ```bash
   nm meeting | grep retrieval
   ```

2. The program expected a shared library exporting `meeting()`. I used `dlsym(RTLD_DEFAULT, "retrieval")` to resolve the global `pthread_t` symbol holding the retrieval thread ID.

3. Inside `meeting()`, I joined that thread and printed its returned pointer as a string. This recovered the flag from the thread's result.

   ```c
   #define _GNU_SOURCE
   #include <dlfcn.h>
   #include <pthread.h>
   #include <stdio.h>

   void meeting(void) {
       pthread_t *retrieval_ptr =
           (pthread_t *)dlsym(RTLD_DEFAULT, "retrieval");
       if (!retrieval_ptr) {
           fprintf(stderr, "retrieval symbol not found\n");
           return;
       }
       void *flag = NULL;
       if (pthread_join(*retrieval_ptr, &flag) == 0 && flag)
           printf("FLAG: %s\n", (char *)flag);
   }
   ```

4. I compiled the shared library and ran the challenge executable.

   ```bash
   gcc -shared -fPIC -o libmeeting.so meeting_hook.c -lpthread -ldl
   ./meeting
   ```

5. The successful run revealed the flag.

   ```text
   threadctf{5HOo7Ing_sOM3_At0m1c_f3et}
   ```

## Подробный разбор

### 1. Я исследовала исполняемый файл

В `needle` флаг извлекался отдельным потоком. Я обнаружила глобальный символ `retrieval`, связанный с идентификатором потока `pthread_t`.

```bash
nm meeting | grep retrieval
```

### 2. Я использовала динамическое разрешение символа

`dlsym(RTLD_DEFAULT, "retrieval")` позволяет найти адрес доступного глобального символа в текущем процессе. Я преобразовала этот адрес к `pthread_t *`.

### 3. Я дождалась потока и прочитала результат

`pthread_join(thread, &result)` ждёт завершения потока и записывает значение, которое поток вернул, по адресу `result`. В успешной попытке этот результат содержал строку флага.

### 4. Я собрала библиотеку

```bash
gcc -shared -fPIC -o libmeeting.so meeting_hook.c -lpthread -ldl
./meeting
```

Опция `-shared` создаёт разделяемую библиотеку, `-fPIC` генерирует позиционно-независимый код, `-lpthread` и `-ldl` подключают необходимые функции потоков и динамической загрузки.

### 5. Я получила флаг

```text
threadctf{5HOo7Ing_sOM3_At0m1c_f3et}
```

**Главная идея:** вместо поиска файла я получила значение, возвращаемое существующим потоком, через `pthread_join()`.
