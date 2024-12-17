#!/bin/bash

build_dir="./build"
model_dir="./models/fixate_experiment"
mkdir "$model_dir" 
serialModel="exp_serial_1024_1500"
SerialLog="$serialModel.txt"
taskset -c 0 "${build_dir}/test_serial" "$serialModel"
 
mv "${serialModel}" "$model_dir"
mv "${SerialLog}" "$model_dir"
echo "Run completed with parameter: $serialModel"

# Parallel runs
threadNum=8
parallelModel="exp_openmp_1024_1500"
parallelLog="$parallelModel.txt"

taskset -c 0-3 "${build_dir}/test_openmp" "$parallelModel" $threadNum

mv "$parallelModel" "$model_dir"
mv "$parallelLog" "$model_dir"
echo "Run completed with parameter: $parallelModel, $threadNum"
echo "All runs completed."
