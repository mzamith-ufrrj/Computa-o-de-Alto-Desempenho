#!/usr/bin/bash
for ((t = 1; t <= 12; t++))
do
    echo "<Threads $t>" >> err.log
    ./multi-mat-cpu.exec r1000x1000.bin i1000x1000.bin resultado1000.bin log-$t.csv $t 0
    diff r1000x1000.bin resultado1000.bin >> err.log
    ./multi-mat-cpu.exec r2000x2000.bin i2000x2000.bin resultado2000.bin log-$t.csv $t 0
    diff r2000x2000.bin resultado2000.bin >> err.log
    ./multi-mat-cpu.exec r3000x3000.bin i3000x3000.bin resultado3000.bin log-$t.csv $t 0
    diff r3000x3000.bin resultado3000.bin >> err.log
    ./multi-mat-cpu.exec r4000x4000.bin i4000x4000.bin resultado4000.bin log-$t.csv $t 0
    diff r4000x4000.bin resultado4000.bin >> err.log
    echo "</Threads $t>" >> err.log
done
