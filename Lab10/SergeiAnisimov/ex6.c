/* Lab 10 Example 6 | Sergei Anisimov, CSE-04 */
#include <errno.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>

int main(void)
{
    int fd = inotify_init1(IN_CLOEXEC);
    if (fd == -1) {
        perror("inotify_init1");
        return 1;
    }
    if (inotify_add_watch(fd, "/tmp", IN_CREATE) == -1) {
        perror("inotify_add_watch /tmp");
        close(fd);
        return 1;
    }

    _Alignas(struct inotify_event) char buffer[4096];
    for (;;) {
        ssize_t length = read(fd, buffer, sizeof buffer);
        if (length == -1 && errno == EINTR)
            continue;
        if (length <= 0) {
            if (length == -1)
                perror("read");
            close(fd);
            return 1;
        }

        size_t offset = 0;
        while ((size_t)length - offset >= sizeof(struct inotify_event)) {
            const struct inotify_event *event =
                (const struct inotify_event *)(buffer + offset);
            size_t event_size = sizeof *event + event->len;
            if (event_size > (size_t)length - offset)
                break;
            if ((event->mask & IN_CREATE) && !(event->mask & IN_ISDIR)) {
                puts("New file created.");
                close(fd);
                return 0;
            }
            offset += event_size;
        }
    }
}
