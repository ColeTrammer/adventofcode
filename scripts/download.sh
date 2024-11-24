#!/usr/bin/env bash

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

input_dir="input/$year"
file_name=$(printf "input_%02d.txt" "$day")
output_path="$project_root/$input_dir/$file_name"
mkdir -p "$project_root/$input_dir"

url="https://adventofcode.com/$year/day/$day/input"

echo "Downloading input from '$url' to '$output_path'..."
curl -s -b "session=$AOC_SESSION" "$url" >"$output_path"
