#!/bin/bash
# This script is a template for the simulation experiment of different algorithms and different erasure codes. According to this template, you can write your own test scripts.

echo In this experiment, we will test the random replacement recovery algorithm:

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 5 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 6 0
done

for i in {1..300}
do	
	$(pwd)/bin/sa_test cauchy_good 5 4 7 0
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 8 0
done
