#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define PROC_FILE "proc.txt"

static const char *COLORS[] = {
    "\x1b[31m","\x1b[32m","\x1b[33m","\x1b[34m","\x1b[35m","\x1b[36m",
    "\x1b[91m","\x1b[92m","\x1b[93m","\x1b[94m","\x1b[95m","\x1b[96m"
};
static const char *RESET = "\x1b[0m";
static const char *DIM = "\x1b[2m";

typedef struct {
    int pid;
    int at;
    int bt;
    int rem;
    int st;
    int ct;
    int tat;
    int wt;
    int rt;
    int started;
    int done;
} Proc;

typedef struct {
    int pid;
    int start;
    int end;
} Segment;

typedef struct {
    Segment *a;
    int n;
    int cap;
} SegList;

static void seg_push(SegList *s, int pid, int start, int end) {
    if (start >= end) return;
    if (s->n > 0 && s->a[s->n - 1].pid == pid && s->a[s->n - 1].end == start) {
        s->a[s->n - 1].end = end;
        return;
    }
    if (s->n == s->cap) {
        s->cap = s->cap ? s->cap * 2 : 32;
        s->a = (Segment *)realloc(s->a, (size_t)s->cap * sizeof(Segment));
        if (!s->a) { perror("realloc"); exit(1); }
    }
    s->a[s->n++] = (Segment){ pid, start, end };
}

static int read_procs(const char *path, Proc **out, int *out_n) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;

    Proc *p = NULL;
    int n = 0, cap = 0;
    char line[512];

    while (fgets(line, sizeof(line), f)) {
        int pid, at, bt;
        char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '\0' || *s == '\n' || *s == '#') continue;
        if (sscanf(s, "%d %d %d", &pid, &at, &bt) != 3) continue;
        if (bt <= 0 || at < 0) continue;

        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            p = (Proc *)realloc(p, (size_t)cap * sizeof(Proc));
            if (!p) { perror("realloc"); exit(1); }
        }
        p[n++] = (Proc){
            .pid = pid, .at = at, .bt = bt, .rem = bt,
            .st = -1, .ct = 0, .tat = 0, .wt = 0, .rt = 0,
            .started = 0, .done = 0
        };
    }

    fclose(f);
    if (n == 0) { free(p); return 0; }

    *out = p;
    *out_n = n;
    return 1;
}

static int color_idx_for_pid(int pid) {
    if (pid < 0) return -1;
    unsigned u = (unsigned)pid;
    return (int)(u % (sizeof(COLORS)/sizeof(COLORS[0])));
}

static int cmp_at_then_pid(const void *a, const void *b) {
    const Proc *pa = (const Proc *)a;
    const Proc *pb = (const Proc *)b;
    if (pa->at != pb->at) return pa->at - pb->at;
    return pa->pid - pb->pid;
}

static int all_done(Proc *p, int n) {
    for (int i = 0; i < n; i++) if (!p[i].done) return 0;
    return 1;
}

static int next_arrival_time(Proc *p, int n, int now) {
    int best = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (!p[i].done && p[i].at > now && p[i].at < best) best = p[i].at;
    }
    return best == INT_MAX ? -1 : best;
}

static void finalize_metrics(Proc *p, int n) {
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
}

static void run_fcfs(Proc *p, int n, SegList *segs) {
    qsort(p, (size_t)n, sizeof(Proc), cmp_at_then_pid);
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < p[i].at) {
            seg_push(segs, -1, t, p[i].at);
            t = p[i].at;
        }
        p[i].st = t;
        p[i].rt = p[i].st - p[i].at;
        seg_push(segs, p[i].pid, t, t + p[i].bt);
        t += p[i].bt;
        p[i].ct = t;
        p[i].done = 1;
    }
    finalize_metrics(p, n);
}

static void run_sjf_np(Proc *p, int n, SegList *segs) {
    int t = 0;
    int min_at = INT_MAX;
    for (int i = 0; i < n; i++) if (p[i].at < min_at) min_at = p[i].at;
    if (min_at != INT_MAX) t = min_at;

    while (!all_done(p, n)) {
        int best = -1;
        int best_bt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= t) {
                if (p[i].bt < best_bt || (p[i].bt == best_bt && p[i].at < (best >= 0 ? p[best].at : INT_MAX))) {
                    best_bt = p[i].bt;
                    best = i;
                }
            }
        }

        if (best < 0) {
            int na = next_arrival_time(p, n, t);
            if (na < 0) break;
            seg_push(segs, -1, t, na);
            t = na;
            continue;
        }

        p[best].st = t;
        p[best].rt = p[best].st - p[best].at;
        seg_push(segs, p[best].pid, t, t + p[best].bt);
        t += p[best].bt;
        p[best].ct = t;
        p[best].done = 1;
    }
    finalize_metrics(p, n);
}

static void run_srtf(Proc *p, int n, SegList *segs) {
    int t = 0;
    int min_at = INT_MAX;
    for (int i = 0; i < n; i++) if (p[i].at < min_at) min_at = p[i].at;
    if (min_at != INT_MAX) t = min_at;

    while (!all_done(p, n)) {
        int best = -1;
        int best_rem = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= t) {
                if (p[i].rem < best_rem || (p[i].rem == best_rem && p[i].at < (best >= 0 ? p[best].at : INT_MAX))) {
                    best_rem = p[i].rem;
                    best = i;
                }
            }
        }

        if (best < 0) {
            int na = next_arrival_time(p, n, t);
            if (na < 0) break;
            seg_push(segs, -1, t, na);
            t = na;
            continue;
        }

        if (!p[best].started) {
            p[best].started = 1;
            p[best].st = t;
            p[best].rt = p[best].st - p[best].at;
        }

        seg_push(segs, p[best].pid, t, t + 1);
        p[best].rem -= 1;
        t += 1;

        if (p[best].rem == 0) {
            p[best].ct = t;
            p[best].done = 1;
        }
    }
    finalize_metrics(p, n);
}

typedef struct {
    int *a;
    int n;
    int cap;
    int head;
    int tail;
} IntQueue;

static void q_init(IntQueue *q) {
    q->cap = 64;
    q->a = (int *)malloc((size_t)q->cap * sizeof(int));
    if (!q->a) { perror("malloc"); exit(1); }
    q->n = 0; q->head = 0; q->tail = 0;
}

static void q_free(IntQueue *q) { free(q->a); }

static int q_empty(IntQueue *q) { return q->n == 0; }

static void q_push(IntQueue *q, int v) {
    if (q->n == q->cap) {
        int newcap = q->cap * 2;
        int *b = (int *)malloc((size_t)newcap * sizeof(int));
        if (!b) { perror("malloc"); exit(1); }
        for (int i = 0; i < q->n; i++) b[i] = q->a[(q->head + i) % q->cap];
        free(q->a);
        q->a = b;
        q->cap = newcap;
        q->head = 0;
        q->tail = q->n;
    }
    q->a[q->tail] = v;
    q->tail = (q->tail + 1) % q->cap;
    q->n++;
}

static int q_pop(IntQueue *q) {
    if (q->n == 0) return -1;
    int v = q->a[q->head];
    q->head = (q->head + 1) % q->cap;
    q->n--;
    return v;
}

static void sort_indices_by_at(Proc *p, int n, int *idx) {
    for (int i = 0; i < n; i++) idx[i] = i;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int a = idx[i], b = idx[j];
            if (p[a].at > p[b].at || (p[a].at == p[b].at && p[a].pid > p[b].pid)) {
                int tmp = idx[i];
                idx[i] = idx[j];
                idx[j] = tmp;
            }
        }
    }
}

static void run_rr(Proc *p, int n, int quantum, SegList *segs) {
    IntQueue rq;
    q_init(&rq);

    int *order = (int *)malloc((size_t)n * sizeof(int));
    if (!order) { perror("malloc"); exit(1); }
    sort_indices_by_at(p, n, order);

    int t = 0;
    int k = 0;
    int min_at = INT_MAX;
    for (int i = 0; i < n; i++) if (p[i].at < min_at) min_at = p[i].at;
    if (min_at != INT_MAX) t = min_at;

    while (k < n && p[order[k]].at <= t) q_push(&rq, order[k++]);

    while (!all_done(p, n)) {
        if (q_empty(&rq)) {
            int na = (k < n) ? p[order[k]].at : -1;
            if (na < 0) break;
            if (t < na) seg_push(segs, -1, t, na);
            t = na;
            while (k < n && p[order[k]].at <= t) q_push(&rq, order[k++]);
            continue;
        }

        int i = q_pop(&rq);
        if (p[i].done) continue;

        if (!p[i].started) {
            p[i].started = 1;
            p[i].st = t;
            p[i].rt = p[i].st - p[i].at;
        }

        int slice = p[i].rem < quantum ? p[i].rem : quantum;
        int end = t + slice;

        seg_push(segs, p[i].pid, t, end);
        p[i].rem -= slice;
        t = end;

        while (k < n && p[order[k]].at <= t) q_push(&rq, order[k++]);

        if (p[i].rem == 0) {
            p[i].ct = t;
            p[i].done = 1;
        } else {
            q_push(&rq, i);
        }
    }

    finalize_metrics(p, n);
    free(order);
    q_free(&rq);
}

static int makespan(SegList *segs) {
    if (segs->n == 0) return 0;
    return segs->a[segs->n - 1].end;
}

static void print_gantt(SegList *segs) {
    int end = makespan(segs);
    printf("\n\x1b[1mGantt\x1b[0m\n");

    int t = 0;
    int si = 0;
    while (t < end) {
        int pid = -1;
        if (si < segs->n) {
            Segment s = segs->a[si];
            if (t >= s.start && t < s.end) pid = s.pid;
            if (t + 1 == s.end) si++;
        }
        if (pid < 0) {
            printf("%s.%s", DIM, RESET);
        } else {
            int ci = color_idx_for_pid(pid);
            printf("%s█%s", COLORS[ci], RESET);
        }
        t++;
    }

    printf("\n");
    printf("0");
    int last = 0;
    for (int i = 0; i < segs->n; i++) {
        int e = segs->a[i].end;
        int pad = e - last;
        for (int k = 0; k < pad - 1; k++) printf(" ");
        printf("%d", e);
        last = e;
    }
    printf("\n\n");

    for (int i = 0; i < segs->n; i++) {
        Segment s = segs->a[i];
        if (s.pid < 0) {
            printf("%s[IDLE %d-%d]%s ", DIM, s.start, s.end, RESET);
        } else {
            int ci = color_idx_for_pid(s.pid);
            printf("%s[P%d %d-%d]%s ", COLORS[ci], s.pid, s.start, s.end, RESET);
        }
    }
    printf("\n\n");
}

static void print_stats(Proc *p, int n) {
    long sum_wt = 0, sum_tat = 0, sum_rt = 0;

    printf("\x1b[1mStats\x1b[0m\n");
    printf("%-6s %-4s %-4s %-4s %-4s %-5s %-5s %-4s\n", "PID", "AT", "BT", "ST", "CT", "TAT", "WT", "RT");

    for (int i = 0; i < n; i++) {
        int ci = color_idx_for_pid(p[i].pid);
        printf("%s%-6d%s %-4d %-4d %-4d %-4d %-5d %-5d %-4d\n",
               COLORS[ci], p[i].pid, RESET,
               p[i].at, p[i].bt, p[i].st, p[i].ct, p[i].tat, p[i].wt, p[i].rt);

        sum_wt += p[i].wt;
        sum_tat += p[i].tat;
        sum_rt += p[i].rt;
    }

    printf("\n");
    printf("Avg WT:  %.2f\n", n ? (double)sum_wt / (double)n : 0.0);
    printf("Avg TAT: %.2f\n", n ? (double)sum_tat / (double)n : 0.0);
    printf("Avg RT:  %.2f\n", n ? (double)sum_rt / (double)n : 0.0);
    printf("\n");
}

static void usage(const char *argv0) {
    printf("Usage: %s <algo>\n", argv0);
    printf("  1 = FCFS\n");
    printf("  2 = SJF (non-preemptive)\n");
    printf("  3 = SRTF (preemptive)\n");
    printf("  4 = Round Robin\n");
}

int main(int argc, char **argv) {
    int algo = 0;
    int quantum = 2;

    if (argc >= 2) {
        algo = atoi(argv[1]);
    } else {
        usage(argv[0]);
        printf("Enter algorithm number: ");
        if (scanf("%d", &algo) != 1) return 1;
    }

    Proc *p = NULL;
    int n = 0;
    if (!read_procs(PROC_FILE, &p, &n)) {
        fprintf(stderr, "Failed to read %s\n", PROC_FILE);
        return 1;
    }

    SegList segs = {0};

    if (algo == 1) {
        run_fcfs(p, n, &segs);
    } else if (algo == 2) {
        run_sjf_np(p, n, &segs);
    } else if (algo == 3) {
        run_srtf(p, n, &segs);
    } else if (algo == 4) {
        printf("Enter quantum (default 2): ");
        int qin = 0;
        if (scanf("%d", &qin) == 1 && qin > 0) quantum = qin;
        run_rr(p, n, quantum, &segs);
    } else {
        usage(argv[0]);
        free(p);
        free(segs.a);
        return 1;
    }

    print_gantt(&segs);
    print_stats(p, n);

    free(p);
    free(segs.a);
    return 0;
}

