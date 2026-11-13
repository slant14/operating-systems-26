#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
build=$(mktemp -d)
watch="$build/watched"
mkdir -- "$watch"
monitor_pid=
cleanup() {
    if [[ -n $monitor_pid ]]; then
        kill -INT "$monitor_pid" 2>/dev/null || true
        wait "$monitor_pid" 2>/dev/null || true
    fi
    rm -f -- "$build/ex1" "$build/monitor" "$build/monitor.log" "$build/myfile1.txt" \
        "$watch/"{myfile1.txt,myfile11.txt,myfile12.txt,myfile13.txt,test-file,test-link}
    if [[ -d $watch/test-dir ]]; then rmdir -- "$watch/test-dir"; fi
    rmdir -- "$watch" "$build"
}
trap cleanup EXIT
for program in monitor ex1; do
    gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror "$src/$program.c" -o "$build/$program"
done
"$build/monitor" "$watch" > "$build/monitor.log" &
monitor_pid=$!
for (( attempt=0; attempt<100; ++attempt )); do
    if grep -q '^Monitoring started$' "$build/monitor.log"; then break; fi
    sleep .02
done
grep -q '^Monitoring started$' "$build/monitor.log"
"$build/ex1" "$watch" "$build/myfile1.txt"
bash "$src/ex1_test.sh" "$watch"
kill -INT "$monitor_pid"
wait "$monitor_pid"
monitor_pid=
cat "$build/monitor.log"
