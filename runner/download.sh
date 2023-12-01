#!/bin/bash

usage() {
    echo "Usage: $0 <year> <day>"
    echo "NOTE: The AOC_SESSION environment variable must be set to your session cookie."
    exit 1
}

if [ $# -ne 2 ]; then
    usage
fi
if [ -z "$AOC_SESSION" ]; then
    usage
fi

script_dir=$(dirname "$0")
project_root=$(realpath "$script_dir/..")

year=$1
day=$2

problem_dir=`printf "%s/%02d" $year $day`
absolute_problem_dir="$project_root/$problem_dir"

url="https://adventofcode.com/$year/day/$day/input"
echo "Downloading input from '$url' to '$absolute_problem_dir/input.txt'..."

curl -s -b "session=$AOC_SESSION" "$url" > "$absolute_problem_dir/input.txt"
