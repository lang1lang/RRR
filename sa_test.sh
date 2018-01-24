#!/bin/bash
# This script is a template for the simulation experiment of different algorithms and different erasure codes. According to this template, you can write your own test scripts.

echo In this experiment, we will test the random replacement recovery algorithm:

code_types=("Cauchy_Orig" "Cauchy_Good" "Liberation" "Blaum_Roth" "Liber8tion")

for i in {1..100}
do
	$(pwd)/bin/sa_test cauchy_orig 5 4 10 0
done
