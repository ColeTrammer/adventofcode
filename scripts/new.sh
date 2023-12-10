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

source_file=`printf "src/%s/%02d.cpp" $year $day`
input_dir="input/$year"
input_file=`printf "input_%02d.txt" $day`
test_file=`printf "test_%02d.txt" $day`

echo "Creating source dir '$project_root/src/$year'..."
mkdir -p "$project_root/src/$year"

echo "Creating source file '$project_root/$source_file'..."
cat > "$project_root/$source_file" << __EOF__
#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/function/not_fn.h>
#include <di/container/view/zip_transform.h>
#include <di/container/view/zip.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION($year, $day, a, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {}
    return s;
}

AOC_SOLUTION($year, $day, b, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {}
    return s;
}
__EOF__

echo "Creating input dir $project_root/$input_dir..."
mkdir -p "$project_root/$input_dir"

echo "Creating input file '$project_root/$input_dir/$input_file'..."
touch "$project_root/$input_dir/$input_file"

echo "Creating test file '$project_root/$input_dir/$test_file'..."
touch "$project_root/$input_dir/$test_file"

echo "Opening $project_root/$source_file..."
code "$project_root/$source_file"

echo "Opening $project_root/$input_dir/$input_file..."
code "$project_root/$input_dir/$input_file"

echo "Opening $project_root/$input_dir/$test_file..."
code "$project_root/$input_dir/$test_file"
