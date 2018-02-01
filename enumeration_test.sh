#!/bin/bash
# This script is a template for the simulation experiment of different algorithms and different erasure codes. According to this template, you can write your own test scripts.

echo In this experiment, we will test the enumeration recovery algorithm:

for i in {1..300}
do
	$(pwd)/bin/enumeration_test blaum_roth 2 2 4 0
done

for i in {1..300}
do
	$(pwd)/bin/enumeration_test blaum_roth 3 2 4 0
done

for i in {1..300}
do
	$(pwd)/bin/enumeration_test cauchy_good 2 2 4 0
done

for i in {1..300}
do
	$(pwd)/bin/enumeration_test cauchy_good 3 2 4 0
done

for i in {1..300}
do
	$(pwd)/bin/enumeration_test cauchy_good 3 3 3 0
done
