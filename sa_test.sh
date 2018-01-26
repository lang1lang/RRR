#!/bin/bash
# This script is a template for the simulation experiment of different algorithms and different erasure codes. According to this template, you can write your own test scripts.

echo In this experiment, we will test the random replacement recovery algorithm:

code_types=("Cauchy_Orig" "Cauchy_Good" "Liberation" "Blaum_Roth" "Liber8tion")

for i in {1..300}
do
	$(pwd)/bin/climb_test liber8tion 2 2 8 0
done

for i in {1..300}
do
	$(pwd)/bin/climb_test liber8tion 4 2 8 0
done

for i in {1..300}
do
	$(pwd)/bin/climb_test blaum_roth 2 2 6 0
done

for i in {1..300}
do
	$(pwd)/bin/climb_test blaum_roth 2 2 10 0
done

for i in {1..300}
do
	$(pwd)/bin/climb_test blaum_roth 8 2 12 0
done

for i in {1..300}
do
	$(pwd)/bin/climb_test blaum_roth 9 2 12 0
done

for i in {1..300}
do
	$(pwd)/bin/climb_test blaum_roth 10 2 12 0
done

for i in {1..300}
do
	$(pwd)/bin/climb_test blaum_roth 11 2 12 0
done

for i in {1..300}
do
	for k in {2..8}
	do 
		for ((m = 2; m <= $k; ++m))
		do
			for w in {2..8}
			do
				$(pwd)/bin/climb_test cauchy_good $k $m $w 0
			done
		done
	done
done
