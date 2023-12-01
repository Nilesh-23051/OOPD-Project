#!/bin/bash

# Create directory (1) with 100 files of 1GB each
mkdir -p dir1
cd dir1
for ((i=1; i<=100; i++)); do
    dd if=/dev/zero of=file_$i bs=1M count=1
done
cd ..



# Create directory (2) with 10000 files of 10MB each
mkdir -p dir2
cd dir2
for ((i=1; i<=10000; i++)); do
    dd if=/dev/zero of=file_$i bs=10M count=1
done
cd ..



# Create directory (3) with only 10MB files and a sub-directory recursively
function create_recursive_dir {
    local current_dir=$1
    local file_count=$2
    local max_files=$3

    if ((file_count >= max_files)); then
        return
    fi

    mkdir -p "$current_dir"
    cd "$current_dir"

    for ((i=1; i<=100; i++)); do
        dd if=/dev/zero of=file_$file_count bs=1M count=1
        ((file_count++))
    done

    for ((i=1; i<=10; i++)); do
        create_recursive_dir "subdir_$i" "$file_count" "$max_files"
    done

    cd ..
}

mkdir -p dir3
cd dir3
create_recursive_dir "subdir_1" 1 10000
cd ..



