#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static int killed[1024];
static int nkilled;

static int was_killed(int pid) {
    for (int i = 0; i < nkilled; i++) if (killed[i] == pid) return 1;
    return 0;
}

static int read_comm(int pid, char *out, size_t cap) {
    char path[80];
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    int ok = fgets(out, cap, f) != NULL;
    fclose(f);
    if (ok) out[strcspn(out, "\n")] = 0;
    return ok;
}

static int zombie_parent(int pid) {
    char path[80], line[256], comm[80];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    int zombie = 0, parent = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "State:", 6) == 0 && strchr(line, 'Z')) zombie = 1;
        if (strncmp(line, "PPid:", 5) == 0) parent = atoi(line + 5);
    }
    fclose(f);
    if (!zombie || !read_comm(pid, comm, sizeof(comm))) return 0;
    if (strncmp(comm, "princess_", 9) != 0) return 0;
    return parent;
}

static int show_reward(void) {
    DIR *d = opendir("/home/ctf");
    if (!d) return 0;
    int found = 0;
    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.' || strncmp(e->d_name, "blade", 5) == 0 ||
            strncmp(e->d_name, "inspect", 7) == 0 ||
            strcmp(e->d_name, "princess2.log") == 0) continue;
        char path[512], buf[1024];
        snprintf(path, sizeof(path), "/home/ctf/%s", e->d_name);
        struct stat st;
        if (stat(path, &st) || !S_ISREG(st.st_mode) || st.st_size > 4096) continue;
        FILE *f = fopen(path, "r");
        if (!f) continue;
        size_t n = fread(buf, 1, sizeof(buf) - 1, f);
        buf[n] = 0;
        printf("FILE %s: %s\n", path, buf);
        found = 1;
        fclose(f);
    }
    closedir(d);
    return found;
}

int main(void) {
    int count = 0;
    for (int pass = 0; pass < 10000 && count < 100; pass++) {
        DIR *d = opendir("/proc");
        if (!d) { perror("/proc"); return 1; }
        struct dirent *e;
        while ((e = readdir(d)) && count < 100) {
            char *end;
            long pid = strtol(e->d_name, &end, 10);
            if (*end || pid <= 1) continue;
            int cabin = zombie_parent((int)pid);
            if (cabin <= 1 || was_killed(cabin)) continue;
            char comm[80];
            if (!read_comm(cabin, comm, sizeof(comm)) ||
                strstr(comm, "cabin") == NULL) continue;
            if (kill(cabin, SIGTERM) == 0) {
                killed[nkilled++] = cabin;
                count++;
                printf("%d/100 cabin=%d princess=%ld\n", count, cabin, pid);
                fflush(stdout);
            }
        }
        closedir(d);
        if (show_reward()) return 0;
        if (count < 100) {
            struct timespec pause_time = {0, 100000000};
            nanosleep(&pause_time, NULL);
        }
    }
    sleep(1);
    show_reward();
    fprintf(stderr, "destroyed=%d\n", count);
    return count == 100 ? 0 : 2;
}
