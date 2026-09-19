#!/bin/sh
# Compile and exercise the warning fixes with errors and memory checks enabled.
set -eu

tests_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd -P)
repo_dir=$(dirname -- "$tests_dir")
output_dir=$(mktemp -d "${TMPDIR:-/tmp}/classbench-regressions.XXXXXX")
trap 'rm -rf -- "$output_dir"' EXIT
trap 'exit 1' HUP INT TERM
test_cxx=${CXX:-clang++}

for generator in db_generator trace_generator; do
    set --
    for source in "$repo_dir/$generator/"*.cc; do
        # The regression test supplies main().
        if [ "$source" != "$repo_dir/$generator/$generator.cc" ]; then
            set -- "$@" "$source"
        fi
    done
    "$test_cxx" -std=c++26 -Werror -O1 -g -fno-omit-frame-pointer \
        -fsanitize=address,undefined -I"$repo_dir/$generator" \
        "$tests_dir/${generator}_regressions.cc" "$@" \
        -o "$output_dir/$generator"
    ASAN_OPTIONS=alloc_dealloc_mismatch=1 UBSAN_OPTIONS=halt_on_error=1 \
        "$output_dir/$generator"
done
