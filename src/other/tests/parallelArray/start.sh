#!/bin/bash

echo "Are you running as root?"

if [ -f ./results_parallel.out ]; 
then
   rm -rf ./results_parallel.out
fi
touch ./results_parallel.out

if [ -f ./results_sequential.out ]; 
then
   rm -rf ./results_sequential.out
fi
touch ./results_sequential.out

for i in `seq 1 7`;
do
	perf stat -e l1d.replacement,l2_lines_out.non_silent,l2_lines_out.useless_pref  ../../cpct/cpct  &>> ./results_parallel.out
	perf stat -e l1d.replacement,l2_lines_out.non_silent,l2_lines_out.useless_pref   ../../cct/cct  &>> ./results_sequential.out
done