#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdint.h>
#include <limits.h>

#define PS_MAX 10
#define QUEUE_SIZE 1024
#define MAX_LINE_LENGTH 1024

typedef struct {
    int idx;
    int at, bt, rt, wt, ct, tat;
    int burst;
    unsigned int remaining_quantum;
} ProcessData;

static int running_process = -1;
static unsigned total_time = 0;
static ProcessData data[PS_MAX];
static pid_t ps[PS_MAX];
static unsigned data_size = 0;
static unsigned int quantum = 0;

static int ps_queue[QUEUE_SIZE];
static int queue_start = 0;
static int queue_end = 0;
static int queue_count = 0;

static int queue_is_empty(void) {
    return queue_count == 0;
}

static void enqueue(int v) {
    if (queue_count >= QUEUE_SIZE) return;
    ps_queue[queue_end] = v;
    queue_end = (queue_end + 1) % QUEUE_SIZE;
    queue_count++;
}

static int peek(void) {
    if (queue_is_empty()) return -1;
    return ps_queue[queue_start];
}

static int dequeue(void) {
    if (queue_is_empty()) return -1;
    int v = ps_queue[queue_start];
    queue_start = (queue_start + 1) % QUEUE_SIZE;
    queue_count--;
    return v;
}

static inline int is_alive(pid_t p) {
    return p > 0;
}

static void resume_process(pid_t p) {
    if (is_alive(p)) kill(p, SIGCONT);
}

static void suspend_process(pid_t p) {
    if (is_alive(p)) kill(p, SIGSTOP);
}

static void terminate_process(pid_t p) {
    if (is_alive(p)) kill(p, SIGTERM);
}

static void read_file(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int line_no = 0;

    memset(data, 0, sizeof(data));
    data_size = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line_no++ == 0) continue;

        int idx, at, bt;
        if (sscanf(line, "%d %d %d", &idx, &at, &bt) != 3) continue;
        if (idx < 0 || idx >= PS_MAX) continue;

        data[idx].idx = idx;
        data[idx].at = at;
        data[idx].bt = bt;
        data[idx].rt = 0;
        data[idx].wt = 0;
        data[idx].ct = 0;
        data[idx].tat = 0;
        data[idx].burst = bt;
        data[idx].remaining_quantum = quantum;

        if ((unsigned)(idx + 1) > data_size) data_size = (unsigned)(idx + 1);
    }

    memset(ps, 0, sizeof(ps));
}

static void create_process(int idx) {
    if (running_process != -1) {
        suspend_process(ps[running_process]);
        running_process = -1;
    }

    pid_t child = fork();
    if (child < 0) return;

    if (child > 0) {
        ps[idx] = child;
        running_process = idx;
        return;
    }

    char idbuf[16];
    snprintf(idbuf, sizeof(idbuf), "%d", idx);
    char *argv[] = {"./worker", idbuf, NULL};
    execvp("./worker", argv);
    _exit(127);
}

static void report(void) {
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;

    for (int i = 0; i < (int)data_size; i++) {
        printf("process %d: \n", i);
        printf("\tat=%d\n", data[i].at);
        printf("\tbt=%d\n", data[i].bt);
        printf("\tct=%d\n", data[i].ct);
        printf("\twt=%d\n", data[i].wt);
        printf("\ttat=%d\n", data[i].tat);
        printf("\trt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %u\n", data_size);
    float avg_wt = data_size ? (float)sum_wt / (float)data_size : 0.0f;
    float avg_tat = data_size ? (float)sum_tat / (float)data_size : 0.0f;
    printf("Average results for this run:\n");
    printf("\tavg_wt=%f\n", avg_wt);
    printf("\tavg_tat=%f\n", avg_tat);
}

static void check_done(void) {
    for (int i = 0; i < (int)data_size; i++) {
        if (data[i].burst > 0) return;
    }
    report();
    exit(EXIT_SUCCESS);
}

static void add_arrivals(void) {
    for (int i = 0; i < (int)data_size; i++) {
        if (data[i].at == (int)total_time && data[i].burst > 0) enqueue(data[i].idx);
    }
}

static int pick_from_queue(void) {
    for (;;) {
        int idx = peek();
        if (idx < 0) return -1;

        if (data[idx].burst == 0) {
            dequeue();
            continue;
        }

        if (data[idx].remaining_quantum == 0) {
            dequeue();
            data[idx].remaining_quantum = quantum;
            enqueue(idx);
            continue;
        }

        return idx;
    }
}

static int find_fallback_process(void) {
    int location = -1;
    int min_at = INT_MAX;

    for (int i = 0; i < (int)data_size; i++) {
        if (data[i].burst > 0 && data[i].at < min_at) {
            min_at = data[i].at;
            location = i;
        }
    }

    return location;
}

static int select_next_idx(void) {
    add_arrivals();

    int idx = pick_from_queue();
    if (idx >= 0) return idx;

    int fallback = find_fallback_process();
    while (fallback >= 0 && (unsigned)data[fallback].at > total_time) {
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, fallback);
        total_time++;
        add_arrivals();
        idx = pick_from_queue();
        if (idx >= 0) return idx;
    }

    return idx >= 0 ? idx : fallback;
}

static void schedule_handler(int signum) {
    (void)signum;
    total_time++;

    if (running_process != -1) {
        data[running_process].burst--;
        if (data[running_process].remaining_quantum > 0) data[running_process].remaining_quantum--;

        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        if (data[running_process].burst == 0) {
            pid_t p = ps[data[running_process].idx];
            terminate_process(p);
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            if (is_alive(p)) waitpid(p, NULL, 0);

            data[running_process].ct = (int)total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            ps[running_process] = 0;
            running_process = -1;
        }
    }

    check_done();

    int next_idx = select_next_idx();
    if (next_idx < 0) return;

    if (running_process != next_idx) {
        if (running_process != -1) {
            data[running_process].remaining_quantum = quantum;
            suspend_process(ps[running_process]);
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            running_process = -1;
        }

        running_process = next_idx;

        if (!is_alive(ps[running_process])) {
            create_process(running_process);
            printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            data[running_process].rt = (int)total_time - data[running_process].at;
        } else {
            resume_process(ps[running_process]);
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Enter size of quantum:\n");
    if (scanf("%u", &quantum) != 1 || quantum == 0) return EXIT_FAILURE;

    memset(ps_queue, -1, sizeof(ps_queue));
    queue_start = queue_end = queue_count = 0;

    FILE *in_file = fopen(argv[1], "r");
    if (!in_file) {
        printf("File is not found or cannot open it!\n");
        return EXIT_FAILURE;
    }

    read_file(in_file);
    fclose(in_file);

    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);
    signal(SIGALRM, schedule_handler);

    for (;;) pause();
}

