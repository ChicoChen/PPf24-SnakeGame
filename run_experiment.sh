#!/bin/bash

build_dir="./build"
model_dir="./models/scheduled_release_8"
mkdir "$model_dir" 
# Serial runs
# for i in {1..10}; do
#     modelFile="exp_serial${i}_1024_1500"
#     modelLog="$modelFile.txt"
#     taskset -c 0 "${build_dir}/test_serial" "$modelFile"
#  
#     mv "${modelFile}" "$model_dir"
#     mv "${modelLog}" "$model_dir"
#     echo "Run $i completed with parameter: $modelFile"
# done

# Parallel runs
threadNum=8
for i in {1..10}; do
    modelFile="exp_openmp${i}_1024_1500"
    modelLog="$modelFile.txt"

    taskset -c 0-7 "${build_dir}/test_openmp" "$modelFile" $threadNum
    mv "$modelFile" "$model_dir"
    mv "$modelLog" "$model_dir"
    echo "Run $i completed with parameter: $modelFile, $threadNum"
done

echo "All runs completed."
