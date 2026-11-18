# fortforks

**Category** threadctf

## Solution

1. I inspected the challenge environment and identified `~/secrets` as a named pipe (FIFO), not a regular file.

   ```bash
   ls -l ~/secrets
   file ~/secrets
   ```

2. I confirmed that the FIFO produced a byte stream when opened for reading.

   ```bash
   head -c 100 ~/secrets
   ```

3. I investigated a C solution using `open()` and `read()` to consume the stream and `fork()` at intervals of approximately three seconds. The idea was to let child processes inherit the already-open file descriptor.

   ```c
   /* Core APIs used in the investigation: */
   /* open("secrets", O_RDONLY); */
   /* read(fd, buffer, sizeof buffer); */
   /* fork(); */
   ```

4. I compiled a local reader to experiment with the stream and process inheritance.

   ```bash
   gcc -o fortforks fortforks.c
   ```

> **Record limitation:** The available conversation history confirms the FIFO investigation and reader approach, but does not preserve the final successful command, output, or flag. Those details are intentionally not invented here.

## Подробный разбор

### 1. Я определила тип файла `secrets`

В отличие от обычного файла, FIFO — это канал для передачи данных между процессами. Чтение может блокироваться до появления процесса-писателя, а данные расходуются по мере чтения.

```bash
ls -l ~/secrets
file ~/secrets
```

### 2. Я проверила поток

```bash
head -c 100 ~/secrets
```

Так я убедилась, что из канала можно получить байты, и стала исследовать поведение программы, которая читает их непрерывно.

### 3. Я использовала системные вызовы C

`open()` открывает FIFO, `read()` читает очередные байты, а `fork()` создаёт дочерний процесс. После `fork()` родитель и ребёнок наследуют дескрипторы, ссылающиеся на одно открытое описание файла. Это важно для поведения чтения из канала.

### 4. Я проверяла идею периодического `fork()`

Я работала над читателем `fortforks.c`, который должен был читать поток и создавать дочерние процессы с интервалом около трёх секунд. Точный финальный листинг и подтверждённый флаг в доступной истории не сохранились, поэтому здесь описан только проверенный ход исследования.
