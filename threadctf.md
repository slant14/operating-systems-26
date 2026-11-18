# Thread CTF solutions by Daniil Chernykh

As in previous solutions I wrote this document by hand, so please be linient with minor mistakes.

## Fortforks

Process can live only 5 seconds so we may reborn it each 3-4 seconds.

```c
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>


#define BUFFER_SIZE 4096
const char pipename[] = "secrets";

time_t born;
int fd = 0;


void reborn() {
	born = time(NULL);
	printf("REBORN at %ld\n", born);
}


void upd() {
	time_t now = time(NULL);
	if ((now - born) >= 3) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(1);
		} else if (pid > 0) exit(0);
		else reborn(); // pid == 0
	}
}

int main(void) {
	reborn();

	fd = open(pipename, O_RDONLY);

	if (fd == -1) {
		perror("open");
		exit(1);
	}

	ssize_t n;
	char buffer[BUFFER_SIZE + 1];

	while (1) {
		n = read(fd, buffer, sizeof(buffer));
		if (n == 0) {
			FILE *success = fopen("success.txt", "w");
			if (success == NULL) {
				perror("open");
				exit(1);
			}
			buffer[BUFFER_SIZE] = '\0';
			fprintf(success, "%s", buffer);
			exit(0);
		} else if (n < 0) {
			perror("read");
			exit(1);
		}
		upd();
	}

	return 0;
}
```

## Princess

Some of princesses are died (meaning they are Zombie processes). All princesses lives in cabins, so that if we destroy the cabin with zombie-princess, we will free they. By the following command we can destroy all cabins with zombie-princesses.

```bash
ps -eo ppid=,stat= | awk '$2 ~ /^Z/ && $1 > 1 && !seen[$1]++ {print $1}' | while read -r pid; do kill -15 "$pid"; done
```

However, there is only 20 cabins in total and only 10 *cursed* cabins. Hopefuly, during the time cabins respawn themselves and we are allowed to repeat the command from above several times. After slow waiting we finally made the forest silent.

```
threadctf{Wh4t_Do_YOU_M34n_thEY_ARe_AlREaDY_Dead}
```

## Princess 2

The idea is the same as for the previous challenge. We need just to rewrite the solution in C and loop the act.

For finding processes I used `dirent.h`. I traversed `/proc` and checked all directory entries names for being an integer using `strtol` from `stdlib.h`.

To get their state (I looked for zombies) and PPID I examined `/proc/PID/stat`. Right after `')'` goes `" %c %d"` which are `state` and `ppid`.

To check if `ppid` belongs to a cabin it is enough to say it is not `1`. (For any process parent either a cabin (for princesses) or `1` (for cabins))

To kill the cabin I used `kill(ppid, SIGTERM` from `signal.h`

```c
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>


/**
 * returns:
 *      0 === OK
 *      -1 === ERR
 */
int get_process_info(int pid, char *state, int *ppid) {
    char path[64];
    char buffer[4096];

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *stat = fopen(path, "r");
    if (!stat) return -1;
    if (!gets(buffer, sizeof(buffer), stat)) {
        fclose(stat);
        return -1;
    }

    fclose(stat);

    char *end = strrchr(buffer, ')');
    if (!end) return -1;
    if (sscanf(end + 1, " %c %d", state, ppid) != 2) return -1;

    return 0;
}

/**
 * In the current task the parent either cabin or root
 */
int is_ppid_cabin(int ppid) {
    return ppid != 1;
}

void kill_parent(int ppid) {
    kill(ppid, SIGTERM);
}

int main(void) {
    int count = 0;

    // Lets traverse /proc seeking zombies
    DIR *proc;

    while (count < 100) {
        proc = opendir("/proc");
        if (proc == NULL) return 1;

        for (;;) {
            if (count >= 100) break;

            char *end;
            long pid;
            char state;
            int ppid;

            struct dirent *direntp = readdir(proc);
            if(dirent == NULL) break;

            pid = strtol(direntp->d_name, &end, 10);
            if (end == direntp->d_name || *end != '\0' || pid <= 0 || pid > INT_MAX) continue;

            get_process_info(pid, &state, &ppid);

            if (state != 'Z') continue;
            if (!is_ppid_cabin(ppid)) continue;

            kill_parent(ppid);
            count++;
            printf("Princesses freed: %d\n", count);
        }
        closedir(proc);

        if (count >= 100) break;

        printf("Wait...\n")
        sleep(1);
    }

    printf("Done.\n");

    return 0;
}
```
