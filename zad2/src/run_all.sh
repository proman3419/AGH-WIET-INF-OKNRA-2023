#!/bin/bash

function run_all() {
    arr=("$@")
    for i in "${arr[@]}"; do
        version="LU_decompose_${i}"
        echo ">>>>> Running make run VERSION=$version"
        make run VERSION="$version"
    done
}

declare -a versions_ids=("1" "2" "3" "4" "5" "6" "7" "8")

echo ">>>>> all"
run_all "${versions_ids[@]}"
