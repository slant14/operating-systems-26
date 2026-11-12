#!/usr/bin/env bash
# Lab 05 Sergei Anisimov CSE-04
set -euo pipefail

if [[ ${1:-} != [1-3] || $# != 1 ]]; then
    printf 'Usage: %s subscriber_count (1–3)\n' "$0" >&2
    exit 1
fi
if ! command -v xterm > /dev/null || [[ -z ${DISPLAY:-} ]]; then
    printf 'An X display and xterm are needed to open separate terminal windows.\n' >&2
    exit 1
fi

src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
build=$(mktemp -d)
trap 'rm -f -- "$build/publisher" "$build/subscriber"; rmdir -- "$build"' EXIT
gcc -std=c11 -Wall -Wextra -Wpedantic "$src/publisher.c" -o "$build/publisher"
gcc -std=c11 -Wall -Wextra -Wpedantic "$src/subscriber.c" -o "$build/subscriber"

export LAB05_FIFO_DIR="${LAB05_FIFO_DIR:-/tmp/ex1}"
xterm -T 'Publisher' -e "$build/publisher" "$1" &
publisher_window=$!
for (( attempt=0; attempt<100; ++attempt )); do
    [[ -p $LAB05_FIFO_DIR/s$1 ]] && break
    kill -0 "$publisher_window" 2>/dev/null || break
    sleep 0.05
done
if [[ ! -p $LAB05_FIFO_DIR/s$1 ]]; then
    printf 'Publisher did not create all FIFOs.\n' >&2
    exit 1
fi

windows=()
for (( id=1; id<=$1; ++id )); do
    xterm -T "Subscriber $id" -e "$build/subscriber" "$id" &
    windows+=("$!")
done
wait "$publisher_window"
for window in "${windows[@]}"; do
    wait "$window"
done
