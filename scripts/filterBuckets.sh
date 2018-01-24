#!/bin/bash

#Loop over 2^5 to 2^20 varying the number of hash functions to discover the best fit for the selectivity
for i in `seq 14 22`;
do
	bucketsSize=$((2**i))
	for j in `seq 0 4`;
	do
		outputName="/home/flav/mestrado/BloomJoin/scripts/filterBucketsResults/new/_${i}_${bucketsSize}_${j}.out"
		./BloomFilterx86 ${bucketsSize} ${j} &>> ${outputName}
	done
done