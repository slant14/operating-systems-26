/* Lab 06 Sergei Anisimov CSE-04 */
#ifndef SCHEDULING_H
#define SCHEDULING_H

#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

enum { MAX_JOBS = 10 };
enum Policy { FCFS, SJF, ROUND_ROBIN };

struct Job {
    int id, arrival, burst, remaining, first, completion;
    pid_t pid;
    bool admitted;
};

static int compare_jobs(const void *left, const void *right)
{
    const struct Job *a = left;
    const struct Job *b = right;
    if (a->arrival != b->arrival)
        return (a->arrival > b->arrival) - (a->arrival < b->arrival);
    return (a->id > b->id) - (a->id < b->id);
}

static int read_jobs(const char *path, struct Job jobs[])
{
    FILE *file = fopen(path, "r");
    if (!file) {
        perror(path);
        return -1;
    }
    char *line = NULL;
    size_t capacity = 0;
    int count = 0;
    if (getline(&line, &capacity, file) == -1)
        count = -1;
    while (count >= 0 && getline(&line, &capacity, file) != -1) {
        int id, arrival, burst;
        char extra;
        int fields = sscanf(line, "%d %d %d %c", &id, &arrival, &burst, &extra);
        if (fields == EOF)
            continue;
        if (count == MAX_JOBS || fields != 3 || id < 0 || arrival < 0 ||
            arrival > 1000000 || burst < 1 || burst > 1000000) {
            count = -1;
            break;
        }
        for (int i = 0; i < count; ++i) {
            if (jobs[i].id == id) {
                count = -1;
                break;
            }
        }
        if (count < 0)
            break;
        jobs[count++] = (struct Job){.id=id, .arrival=arrival, .burst=burst,
                                     .remaining=burst, .first=-1};
    }
    if (ferror(file))
        count = -1;
    free(line);
    fclose(file);
    if (count > 0)
        qsort(jobs, (size_t)count, sizeof *jobs, compare_jobs);
    return count;
}

static int wait_for(pid_t pid, int options, int *status)
{
    pid_t result;
    do {
        result = waitpid(pid, status, options);
    } while (result == -1 && errno == EINTR);
    return result == pid ? 0 : -1;
}

static int start_worker(struct Job *job)
{
    if (job->pid == 0) {
        char id[32];
        snprintf(id, sizeof id, "%d", job->id);
        job->pid = fork();
        if (job->pid == -1) {
            job->pid = 0;
            return -1;
        }
        if (job->pid == 0) {
            char *arguments[] = {"./worker", id, NULL};
            execvp(arguments[0], arguments);
            perror("worker");
            _exit(127);
        }
        int status;
        if (wait_for(job->pid, WUNTRACED, &status) == -1)
            return -1;
        if (!WIFSTOPPED(status)) {
            job->pid = 0;
            return -1;
        }
    }
    return kill(job->pid, SIGCONT);
}

static int stop_worker(struct Job *job)
{
    int status;
    if (kill(job->pid, SIGTSTP) == -1 || wait_for(job->pid, WUNTRACED, &status) == -1)
        return -1;
    if (WIFSTOPPED(status))
        return 0;
    job->pid = 0;
    return -1;
}

static int finish_worker(struct Job *job)
{
    int status;
    if (kill(job->pid, SIGTERM) == -1 || wait_for(job->pid, 0, &status) == -1)
        return -1;
    job->pid = 0;
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : -1;
}

static int select_job(const struct Job jobs[], int count, enum Policy policy, int time)
{
    int selected = -1;
    for (int i = 0; i < count; ++i) {
        if (jobs[i].arrival > time || jobs[i].remaining == 0)
            continue;
        if (selected == -1 || (policy == SJF && jobs[i].burst < jobs[selected].burst))
            selected = i;
    }
    return selected;
}

static int simulate(struct Job jobs[], int count, enum Policy policy, int quantum,
                    const sigset_t *signals)
{
    int time = 0, active = -1, used = 0, finished = 0;
    int queue[MAX_JOBS], head = 0, queued = 0, requeue = -1;
    while (finished < count) {
        if (policy == ROUND_ROBIN) {
            for (int i = 0; i < count; ++i) {
                if (!jobs[i].admitted && jobs[i].arrival <= time) {
                    queue[(head + queued++) % MAX_JOBS] = i;
                    jobs[i].admitted = true;
                }
            }
            if (requeue >= 0) {
                queue[(head + queued++) % MAX_JOBS] = requeue;
                requeue = -1;
            }
        }

        if (active == -1) {
            if (policy == ROUND_ROBIN) {
                if (queued) {
                    active = queue[head];
                    head = (head + 1) % MAX_JOBS;
                    --queued;
                }
            } else {
                active = select_job(jobs, count, policy, time);
            }
            if (active >= 0) {
                if (jobs[active].first == -1)
                    jobs[active].first = time;
                used = 0;
                printf("t=%d dispatch %d\n", time, jobs[active].id);
                if (start_worker(&jobs[active]) == -1)
                    return 1;
            }
        }

        int signal_number;
        do {
            signal_number = sigwaitinfo(signals, NULL);
        } while (signal_number == -1 && errno == EINTR);
        if (signal_number != SIGALRM)
            return 1;
        ++time;
        if (active == -1)
            continue;

        struct Job *job = &jobs[active];
        if (--job->remaining == 0) {
            if (finish_worker(job) == -1)
                return 1;
            job->completion = time;
            ++finished;
            active = -1;
        } else if (policy == ROUND_ROBIN && ++used == quantum) {
            if (stop_worker(job) == -1)
                return 1;
            requeue = active;
            active = -1;
        }
    }
    return 0;
}

static int run_scheduler(int argc, char **argv, enum Policy policy)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s data.txt\n", argv[0]);
        return 2;
    }
    int quantum = 1;
    if (policy == ROUND_ROBIN) {
        puts("Enter the time quantum:");
        if (scanf("%d", &quantum) != 1 || quantum < 1) {
            fputs("Invalid quantum.\n", stderr);
            return 2;
        }
    }
    struct Job jobs[MAX_JOBS];
    int count = read_jobs(argv[1], jobs);
    if (count < 1) {
        fputs("Expected one to ten valid jobs.\n", stderr);
        return 1;
    }

    unsigned long tick = 1000000;
    const char *override = getenv("LAB06_TICK_US");
    if (override) {
        char *end;
        errno = 0;
        unsigned long value = strtoul(override, &end, 10);
        if (errno || end == override || *end || value < 1000 || value > 1000000) {
            fputs("LAB06_TICK_US must be between 1000 and 1000000.\n", stderr);
            return 2;
        }
        tick = value;
    }
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGALRM);
    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGTERM);
    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }
    setvbuf(stdout, NULL, _IOLBF, 0);
    struct timeval interval = {.tv_sec = (time_t)(tick / 1000000),
                               .tv_usec = (suseconds_t)(tick % 1000000)};
    struct itimerval timer = {.it_interval = interval, .it_value = interval};
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }
    int result = simulate(jobs, count, policy, quantum, &signals);
    const struct itimerval off = {0};
    setitimer(ITIMER_REAL, &off, NULL);
    if (result) {
        fputs("Scheduler stopped before completing all jobs.\n", stderr);
        for (int i = 0; i < count; ++i) {
            if (jobs[i].pid > 0) {
                kill(jobs[i].pid, SIGKILL);
                wait_for(jobs[i].pid, 0, NULL);
            }
        }
        return 1;
    }

    double total_turnaround = 0, total_waiting = 0;
    puts("idx at bt rt ct tat wt");
    for (int i = 0; i < count; ++i) {
        struct Job job = jobs[i];
        int turnaround = job.completion - job.arrival;
        int waiting = turnaround - job.burst;
        printf("%d %d %d %d %d %d %d\n", job.id, job.arrival, job.burst,
               job.first - job.arrival, job.completion, turnaround, waiting);
        total_turnaround += turnaround;
        total_waiting += waiting;
    }
    printf("Average turnaround: %.2f\nAverage waiting: %.2f\n",
           total_turnaround / count, total_waiting / count);
    return 0;
}
#endif
