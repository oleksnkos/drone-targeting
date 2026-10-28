#!/usr/bin/env bash
set -e

mkdir -p output

for mission in input/*.txt; do
    base=$(basename "$mission" .txt)
    ./drop-calc --input-file "$mission" --output-file "output/${base}-result.txt"
    ./drop-calc --input-file "$mission" --output-file "output/${base}-result.txt" --trajectory forward
done