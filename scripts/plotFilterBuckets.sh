#!/bin/bash
index=0.557142857
control=-1
for file in /home/flav/mestrado/BloomJoin/scripts/filterBucketsResults/*;
do
	nBuckets=$(echo $file | grep -m 1 "Buckets" | cut -d "_" -f2)
	nHash=$(cat $file | grep -m 1 "Buckets" | cut -d " " -f3)

	execTime=$(tac $file | grep -m 1 "ms" | cut -d " " -f1)
	nRegisters=$(cat $file | grep -m 1 "linhas" | cut -d " " -f1)
	nRegistersPerc=$(echo "(${nRegisters}/50004)*100" | bc -l)

	outputFile="/home/flav/mestrado/BloomJoin/gnu/filterBuckets/_${nHash}.dat"
	outputFile2="/home/flav/mestrado/BloomJoin/gnu/filterBuckets/time.dat"

	nBucketsNew=""
	for char in $(echo "${nBuckets}" | fold -w1);
	do
		nBucketsNew="${nBucketsNew}^${char}"
	done

	index=$(echo "${index}+0.142857143"| bc -l)
	if [ "$control" -lt "4" ];
	then
		control=$((control+1))
	else
		control=0
		index=$(echo "${index}+0.142857143+0.142857143"| bc -l)
	fi;

	echo "2${nBucketsNew}	${nHash}	${nRegistersPerc}	${execTime}" &>> ${outputFile}
	echo "${index}	${execTime}" &>> ${outputFile2}
done