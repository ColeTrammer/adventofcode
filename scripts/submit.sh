#!/bin/bash

usage() {
    echo "Usage: $0"
    echo "NOTE: The AOC_SESSION environment variable must be set to your session cookie."
    echo "NOTE: This script expects input of the form:"
    echo "      YYYY day DD part [a|b]: ANSWER"
    exit 1
}

if [ -z "$AOC_SESSION" ]; then
    usage
fi

script_dir=$(dirname "$0")
project_root=$(realpath "$script_dir/..")

while read -r line; do
    year=$(echo "$line" | cut -d' ' -f 1)
    day=$(echo "$line" | cut -d' ' -f 3)
    part=$(echo "$line" | cut -d' ' -f 5 | tr -d ':')
    answer=$(echo "$line" | cut -d' ' -f 6)

    part_number=1
    if [ "$part" = "b" ]; then
        part_number=2
    fi
    url="https://adventofcode.com/$year/day/$day/answer"

    echo "Submitting '$answer' for $year day $day part $part..."
    response=`curl -X POST \
        -H 'Content-Type: application/x-www-form-urlencoded' \
        -s -b "session=$AOC_SESSION" \
        -d "level=$part_number&answer=$answer" \
        "$url" |
    grep '<article>' |
    sed -e 's/<[^>]*>//g'`

    echo $response

    if grep -Eq "That's the right answer!" <<< "$response";
    then
        if [ $part = "a" ]; then
            echo 'Opening part 2...'
            "$script_dir/open.sh" "$year" "$day" "b"
        fi
    fi
done
