#!/bin/bash

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

problem_dir=`printf "%s/%02d" $year $day`
absolute_problem_dir="$project_root/$problem_dir"
echo "Creating '$absolute_problem_dir'..."

mkdir -p "$absolute_problem_dir"
touch "$absolute_problem_dir/input.txt"
touch "$absolute_problem_dir/test.txt"

cat > "$absolute_problem_dir/solution.cpp" << __EOF__
#include <runner/aoc_problem_registry.h>

#include <dius/print.h>

AOC_SOLUTION(${year}, ${day}, a, i32) {
    auto lines = input | di::split(U'\n');
    return 0;
}

AOC_SOLUTION(${year}, ${day}, b, i32) {
    auto lines = input | di::split(U'\n');
    return 0;
}
__EOF__

echo "Adding $problem_dir to '$project_root/runner/problem_list.txt'..."
echo "$problem_dir" >> "$project_root/runner/problem_list.txt"

cp "$project_root/runner/problem_list.txt" /tmp/problem_list.txt
sort /tmp/problem_list.txt | uniq > "$project_root/runner/problem_list.txt"
rm -f /tmp/problem_list.txt
