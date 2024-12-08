#!/usr/bin/env bash

usage() {
    echo "Usage: $0"
    echo "NOTE: The AOC_SESSION environment variable must be set to your session cookie."
    exit 1
}

if [ -z "$AOC_SESSION" ]; then
    usage
fi

script_dir=$(dirname "$0")

year=$(date +%Y)
today=$(date +%d)

next_day=$((${today#0} + 1))

"$script_dir/new.sh" "$year" "$next_day"

while true; do
    hour=$(date +%H)
    minute=$(date +%M)
    second=$(date +%S)

    if [ "$hour" -eq 21 ] && [ "$minute" -ge 0 ] && [ "$second" -ge 0 ]; then
        "$script_dir/open.sh" "$year" "$next_day"
        "$script_dir/download_input.sh" "$year" "$next_day"
        "$script_dir/download_test.sh" "$year" "$next_day"
        break
    fi

    sleep 1
done
