#!/bin/bash

for ((therun=14460;therun<=14510;therun++)); do
    echo "Analyzing Run ${therun}"
    source runAll.sh ${therun} 1> log_run${therun}.txt 2> err_run${therun}.txt
done
