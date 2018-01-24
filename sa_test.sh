#!/bin/bash
# This script is a template for the simulation experiment of different algorithms and different erasure codes. According to this template, you can write your own test scripts.

echo In this experiment, we will test the random replacement recovery algorithm:

code_types=("Cauchy_Orig" "Cauchy_Good" "Liberation" "Blaum_Roth" "Liber8tion")

for i in {1..100}
do
	$(pwd)/bin/climb_test liber8tion 2 2 8 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test liber8tion 3 2 8 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test liber8tion 4 2 8 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test liber8tion 5 2 8 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test liber8tion 6 2 8 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test liber8tion 7 2 8 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test blaum_roth 2 2 6 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test blaum_roth 2 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test blaum_roth 2 2 12 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test blaum_roth 2 2 16 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 2 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 3 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 4 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 5 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 6 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 7 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 9 2 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 5 4 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 6 4 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 7 4 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 4 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 3 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 4 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 5 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 6 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 4 10 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 4 11 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 4 12 0
done

for i in {1..100}
do
	$(pwd)/bin/climb_test cauchy_good 8 4 13 0
done
