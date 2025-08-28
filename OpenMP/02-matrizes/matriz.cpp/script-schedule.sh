#!/usr/bin/bash
for ((t = 1; t <= 10; t++))
do
    #./multi-mat-cpu-dynamic.exe r4000x4000.bin i4000x4000.bin resultado4000-dynamic.bin  log-6-dynamic-Xeon-E-2226G.csv 6 0
    diff r4000x4000.bin resultado4000-dynamic.bin >> err.log
    #./multi-mat-cpu-guided.exe r4000x4000.bin i4000x4000.bin resultado4000-guided.bin  log-6-guided-Xeon-E-2226G.csv 6 0
    diff r4000x4000.bin resultado4000-guided.bin >> err.log
    #./multi-mat-cpu-static.exe r4000x4000.bin i4000x4000.bin resultado-static.bin   log-6-static-Xeon-E-2226G.csv 6 0
    diff r4000x4000.bin resultado4000-static.bin >> err.log

done
