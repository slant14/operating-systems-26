/* Lab 12 Exercise 1 | Sergei Anisimov, CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

struct Shortcut {
    unsigned short keys[6];
    size_t size;
    const char *label;
    const char *message;
};

static const struct Shortcut shortcuts[] = {
    {{KEY_P, KEY_E}, 2, "P+E", "I passed the Exam!"},
    {{KEY_C, KEY_A, KEY_P}, 3, "C+A+P", "Get some cappuccino!"},
    {{KEY_O, KEY_S}, 2, "O+S", "Operating Systems!"},
    {{KEY_E, KEY_X}, 2, "E+X", NULL}
};

/* Sample presses, releases and one repeat, followed by the exit chord. */
static const struct input_event demo[] = {
    {.type = EV_SYN, .code = SYN_REPORT},
    {.type = EV_KEY, .code = KEY_P, .value = 1},
    {.type = EV_KEY, .code = KEY_E, .value = 1},
    {.type = EV_KEY, .code = KEY_E, .value = 2},
    {.type = EV_KEY, .code = KEY_E, .value = 0},
    {.type = EV_KEY, .code = KEY_P, .value = 0},
    {.type = EV_KEY, .code = KEY_C, .value = 1},
    {.type = EV_KEY, .code = KEY_A, .value = 1},
    {.type = EV_KEY, .code = KEY_P, .value = 1},
    {.type = EV_KEY, .code = KEY_C, .value = 0},
    {.type = EV_KEY, .code = KEY_A, .value = 0},
    {.type = EV_KEY, .code = KEY_P, .value = 0},
    {.type = EV_KEY, .code = KEY_O, .value = 1},
    {.type = EV_KEY, .code = KEY_S, .value = 1},
    {.type = EV_KEY, .code = KEY_O, .value = 0},
    {.type = EV_KEY, .code = KEY_S, .value = 0},
    {.type = EV_KEY, .code = KEY_E, .value = 1},
    {.type = EV_KEY, .code = KEY_X, .value = 1},
    {.type = EV_KEY, .code = KEY_Z, .value = 1}
};

static bool shortcut_triggered(const struct Shortcut *shortcut, const bool down[],
                               unsigned short pressed)
{
    bool contains_pressed_key = false;
    for (size_t i = 0; i < shortcut->size; ++i) {
        if (!down[shortcut->keys[i]])
            return false;
        if (shortcut->keys[i] == pressed)
            contains_pressed_key = true;
    }
    return contains_pressed_key;
}

/* Read one complete Linux input_event from a recorded test stream. */
static int next_event(int file, struct input_event *event)
{
    size_t offset = 0;
    unsigned char *bytes = (unsigned char *)event;
    while (offset < sizeof *event) {
        ssize_t count = read(file, bytes + offset, sizeof *event - offset);
        if (count == -1) {
            if (errno == EINTR)
                continue;
            perror("read");
            return -1;
        }
        if (count == 0) {
            if (offset == 0)
                return 0;
            fputs("Truncated input_event record.\n", stderr);
            return -1;
        }
        offset += (size_t)count;
    }
    return 1;
}

/* Return false when E+X ends the stream. */
static bool process_event(const struct input_event *event, bool down[])
{
    const char *const type[] = {"RELEASED", "PRESSED", "REPEATED"};
    if (event->type != EV_KEY || event->code >= KEY_CNT ||
        event->value < 0 || event->value > 2)
        return true;

    unsigned short code = event->code;
    printf("%s 0x%04x (%u)\n", type[event->value],
           (unsigned int)code, (unsigned int)code);
    if (event->value == 0) {
        down[code] = false;
        return true;
    }
    /* A repeat is printed above but does not fire a shortcut again. */
    if (event->value != 1 || down[code])
        return true;
    down[code] = true;
    for (size_t i = 0; i < sizeof shortcuts / sizeof shortcuts[0]; ++i) {
        const struct Shortcut *shortcut = &shortcuts[i];
        if (!shortcut_triggered(shortcut, down, code))
            continue;
        if (!shortcut->message)
            return false;
        puts(shortcut->message);
    }
    return true;
}

static void print_shortcuts(void)
{
    puts("Available shortcuts:");
    for (size_t i = 0; i < sizeof shortcuts / sizeof shortcuts[0]; ++i)
        printf("%s: %s\n", shortcuts[i].label,
               shortcuts[i].message ? shortcuts[i].message : "exit");
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [synthetic-input-events.bin]\n", argv[0]);
        return 2;
    }
    bool down[KEY_CNT] = {false};
    if (argc == 1) {
        print_shortcuts();
        for (size_t i = 0; i < sizeof demo / sizeof demo[0]; ++i)
            if (!process_event(&demo[i], down))
                break;
        return 0;
    }

    int file = open(argv[1], O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (file == -1) {
        perror(argv[1]);
        return 1;
    }
    struct stat info;
    /* The input is a fixture file of raw records for reproducible tests. */
    if (fstat(file, &info) == -1 || !S_ISREG(info.st_mode)) {
        fputs("Expected a regular input_event fixture file.\n", stderr);
        close(file);
        return 1;
    }

    print_shortcuts();
    struct input_event event;
    int status;
    while ((status = next_event(file, &event)) > 0)
        if (!process_event(&event, down))
            break;
    close(file);
    return status < 0;
}
