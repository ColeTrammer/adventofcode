#!/bin/bash

usage() {
    echo "Usage: $0 <year> <day> [part]"
    exit 1
}

if [ $# -ne 2 ] && [ $# -ne 3 ]; then
    usage
fi

year=$1
day=$2
part=$3

url="https://adventofcode.com/$year/day/$day"
if [ "$part" = "b" ];
then
    url="$url#part2"
fi

echo "Opening $url..."
xdg-open "$url"
