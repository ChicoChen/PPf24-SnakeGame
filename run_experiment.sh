#!/bin/bash

build_dir="./build"
model_dir="./models/experiments"

# Serial runs
for i in {1..5}; do
    modelFile="exp_serial${i}_1024_1500"
    modelLog="$modelFile.log"
    taskset -c 0 "${build_dir}/test_serial" "$modelFile"

    mv "${modelFile}" "$model_dir"
    mv "${modelLog}" "$model_dir"
    echo "Run $i completed with parameter: $modelFile"
done

# Parallel runs
threadNum=8
for i in {1..5}; do
    modelFile="exp_openmp${i}_1024_1500"
    modelLog="$modelFile.log"

    taskset -c 0-7 "${build_dir}/test_openmp" "$modelFile" $threadNum

    mv "$modelFile" "$model_dir"
    mv "$modelLog" "$model_dir"
    echo "Run $i completed with parameter: $modelFile, $threadNum"
done

echo "All runs completed."