#!/bin/bash

function run_all() {
    arr=("$@")
    for i in "${arr[@]}"; do
        file="lu_decompose_${i}.c"
        echo ">>>>> Running make run FILE=$file"
        make run FILE="$file"
    done
}

declare -a files=("1" "2" "3" "4" "5" "6" "7")

echo ">>>>> 1-7"
run_all "${files[@]}"
