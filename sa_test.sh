#!/bin/bash
# This script is a template for the simulation experiment of different algorithms and different erasure codes. According to this template, you can write your own test scripts.

echo In this experiment, we will test the random replacement recovery algorithm:

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 3 3 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 3 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 3 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 6 3 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 6 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 7 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 8 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 9 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 2 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 3 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 5 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 3 7 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 3 8 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 3 9 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 3 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 4 7 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 4 8 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 4 9 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 4 4 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 3 7 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 3 8 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 3 9 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 3 10 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 7 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 8 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 9 0	
done

for i in {1..300}
do
	$(pwd)/bin/sa_test cauchy_good 5 4 10 0	
done













