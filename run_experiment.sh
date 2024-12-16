#!/bin/bash

# Loop to run the program 5 times with different parameters
for i in {1..5}; do
    modelName="exp${i}_1024_1500"
    threadNum=8
    ./test_ga "$test_ga" $threadNum
    echo "Run $i completed with parameter: $modelName"
done

echo "All runs completed."