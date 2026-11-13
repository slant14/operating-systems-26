/* Lab 10 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>

static volatile sig_atomic_t stopped;

static void stop(int signal_number)
{
    (void)signal_number;
    stopped = 1;
}

static void show_stat(const char *name)
{
    struct stat st;
    if (lstat(name, &st) == -1) {
        if (errno == ENOENT)
            printf("%s: entry no longer exists\n", name);
        else
            perror(name);
        return;
    }
    printf("%s: inode=%ju links=%ju mode=%04o size=%jd uid=%ju gid=%ju mtime=%jd\n",
           name, (uintmax_t)st.st_ino, (uintmax_t)st.st_nlink,
           (unsigned int)st.st_mode & 07777, (intmax_t)st.st_size,
           (uintmax_t)st.st_uid, (uintmax_t)st.st_gid, (intmax_t)st.st_mtime);
}

static int snapshot(const char *heading)
{
    puts(heading);
    show_stat(".");
    DIR *directory = opendir(".");
    if (!directory) {
        perror("opendir");
        return 1;
    }
    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' ||
            (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
            continue;
        show_stat(entry->d_name);
    }
    closedir(directory);
    return 0;
}

static void report(const struct inotify_event *event)
{
    const char *name = event->len ? event->name : ".";
    static const struct { uint32_t mask; const char *label; } types[] = {
        {IN_ACCESS, "accessed"}, {IN_CREATE, "created"}, {IN_DELETE, "deleted"},
        {IN_MODIFY, "modified"}, {IN_OPEN, "opened"}, {IN_ATTRIB, "metadata changed"},
        {IN_MOVED_FROM, "moved out"}, {IN_MOVED_TO, "moved in"},
        {IN_DELETE_SELF, "deleted"}, {IN_MOVE_SELF, "moved"}
    };
    if (event->mask & IN_Q_OVERFLOW)
        fputs("inotify queue overflow\n", stderr);
    for (size_t i = 0; i < sizeof types / sizeof types[0]; ++i) {
        if (event->mask & types[i].mask) {
            printf("%s is %s\n", name, types[i].label);
            show_stat(name);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s directory\n", argv[0]);
        return 2;
    }
    if (chdir(argv[1]) == -1) {
        perror(argv[1]);
        return 1;
    }

    struct sigaction action = {.sa_handler = stop};
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return 1;
    }
    setvbuf(stdout, NULL, _IOLBF, 0);
    int result = snapshot("Initial stat snapshot");
    int fd = inotify_init1(IN_CLOEXEC | IN_NONBLOCK);
    if (fd == -1) {
        perror("inotify_init1");
        return 1;
    }
    uint32_t mask = IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN |
                    IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE_SELF | IN_MOVE_SELF;
    if (inotify_add_watch(fd, ".", mask) == -1) {
        perror("inotify_add_watch");
        close(fd);
        return 1;
    }
    puts("Monitoring started");
    struct pollfd watch = {.fd = fd, .events = POLLIN};
    for (;;) {
        _Alignas(struct inotify_event) char buffer[16384];
        if (!stopped) {
            int ready = poll(&watch, 1, 200);
            if (ready == 0 || (ready == -1 && errno == EINTR))
                continue;
            if (ready == -1) {
                perror("poll");
                result = 1;
                break;
            }
        }
        ssize_t size = read(fd, buffer, sizeof buffer);
        if (size < 0 && (errno == EAGAIN || errno == EINTR)) {
            if (stopped)
                break;
            continue;
        }
        if (size <= 0) {
            perror("inotify read");
            result = 1;
            break;
        }
        for (size_t offset = 0; offset < (size_t)size;) {
            const struct inotify_event *event = (void *)(buffer + offset);
            report(event);
            offset += sizeof *event + event->len;
            if (event->mask & (IN_IGNORED | IN_DELETE_SELF))
                stopped = 1;
        }
    }
    close(fd);
    return snapshot("Final stat snapshot") || result;
}
