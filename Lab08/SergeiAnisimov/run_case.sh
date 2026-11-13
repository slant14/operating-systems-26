#!/usr/bin/env bash
# Lab 08 Sergei Anisimov CSE-04
set -euo pipefail

if (( $# < 3 )); then
    printf 'Usage: %s pages frames reference...\n' "$0" >&2
    exit 2
fi
pages=$1 frames=$2
shift 2
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
build=$(mktemp -d)
pager_pid=
cleanup() {
    if [[ -n $pager_pid ]]; then
        kill "$pager_pid" 2>/dev/null || true
        wait "$pager_pid" 2>/dev/null || true
    fi
    rm -f -- "$build/mmu" "$build/pager" "$build/pagetable" "$build/pager.log"
    rmdir -- "$build"
}
trap cleanup EXIT
export OS_PAGE_TABLE="$build/pagetable"
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror "$src/pager.c" -o "$build/pager"
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror "$src/mmu.c" -o "$build/mmu"
"$build/pager" "$pages" "$frames" > "$build/pager.log" &
pager_pid=$!
for (( attempt=0; attempt<100; ++attempt )); do
    if grep -q '^Pager ready' "$build/pager.log"; then break; fi
    kill -0 "$pager_pid" 2>/dev/null || break
    sleep .03
done
if ! grep -q '^Pager ready' "$build/pager.log"; then
    cat "$build/pager.log" >&2
    exit 1
fi
timeout 20 "$build/mmu" "$pages" "$@" "$pager_pid"
wait "$pager_pid"
pager_pid=
cat "$build/pager.log"
test ! -e "$OS_PAGE_TABLE"
