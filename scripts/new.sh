#!/usr/bin/env bash

usage() {
    echo "Usage: $0 <year> <day>"
    exit 1
}

if [ $# -ne 2 ]; then
    usage
fi

script_dir=$(dirname "$0")
project_root=$(realpath "$script_dir/..")

year=$1
day=$2

source_file="$(printf "src/%s/%02d.cpp" "$year" "$day")"
input_dir="$(printf "input/%d/%02d" "$year" "$day")"
input_file="input.txt"
test_file="test_00.txt"

echo "Creating source dir '$project_root/src/$year'..."
mkdir -p "$project_root/src/$year"

echo "Creating source file '$project_root/$source_file'..."
cat >"$project_root/$source_file" <<__EOF__
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION($year, $day, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {

    }
    return s;
}

AOC_SOLUTION($year, $day, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {

    }
    return s;
}
__EOF__

echo "Creating input dir $project_root/$input_dir..."
mkdir -p "$project_root/$input_dir"

echo "Creating input file '$project_root/$input_dir/$input_file'..."
touch "$project_root/$input_dir/$input_file"

echo "Creating test file '$project_root/$input_dir/$test_file'..."
touch "$project_root/$input_dir/$test_file"
