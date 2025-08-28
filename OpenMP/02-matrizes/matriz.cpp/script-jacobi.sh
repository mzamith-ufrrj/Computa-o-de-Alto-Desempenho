#!/usr/bin/bash
for ((t = 2; t <= 12; t++))
do
    for ((s = 1; s <= 10; s++))
    do
        ./solver-jacobi.exec 1.0E-10 0 A.bin B.bin X1.bin log-xeon-$t.csv $t
        #./solver-jacobi.exec 1.0E-10 0 A.bin B.bin X1.bin log-1.csv 1
    done
done

