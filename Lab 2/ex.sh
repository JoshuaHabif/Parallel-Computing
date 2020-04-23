#!/bin/bash 

module load gcc-9.2

gcc -g -Wall -fopenmp -std=c99 -o genprime genprimes.c -lm

for s in 10000 100000 1000000 10000000 100000000
do
    echo $s

    for t in 1 2 5 10 20 100
    do
	echo $t
	./genprime $s $t
	done
done 
