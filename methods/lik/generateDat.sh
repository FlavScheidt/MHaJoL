#!/bin/bash

metrics=("L2" 
		"L3" 
		"TLB_DATA" 
		"L3CACHE" 
		"ENERGY" 
		"L2CACHE" 
		"BRANCH" 
		"CYCLE_ACTIVITY" 
		"CLOCK" 
		"DATA" 
		"ICACHE" 
		"FALSE_SHARE" 
		"UOPS_RETIRE" 
		"RECOVERY" 
		"UOPS" 
		"UOPS_ISSUE" 
		"UOPS_EXEC")

methods=("bloom" "bloomChained" "bloomNested" "chained" "cuckoo")
region=("Initialization" "Generation" "Core")



for i in `seq 1 9`;
do
	selectivity=$((i*10))
	echo ${selectivity}

	#in directory
	outDir="/home/flav/mestrado/MHaJoL/methods/lik/out/joins/old/_${selectivity}"
	echo "	${outDir}"

	for n in "${methods[@]}";
	do
		for s in `seq 1 3 357`;
		do
			#General Metrics
			RuntimeInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "RDTSC Runtime [s]" |  sed 's/ //g')
			CallCountInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "call count" | cut -d "|" -f3 |  sed 's/ //g')
			InstructionsRetiredInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "INSTR_RETIRED_ANY" | cut -d "|" -f4 |  sed 's/ //g')
			ClockUnhaltedInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPU_CLK_UNHALTED_CORE" | cut -d "|" -f4 |  sed 's/ //g')
			ClockUnhaltedRefInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPU_CLK_UNHALTED_REF" | cut -d "|" -f4 |  sed 's/ //g')
			RuntimeUnhaltedInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "INSTR_RETIRED_ANY" | cut -d "|" -f4 |  sed 's/ //g')
			ClockInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "Clock [MHz]" | cut -d "|" -f3 |  sed 's/ //g')
			CPIInit=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPI" | cut -d "|" -f3 |  sed 's/ //g')

			s=$((s+1))
			
			RuntimeGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "RDTSC Runtime [s]" |  sed 's/ //g')
			CallCountGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "call count" | cut -d "|" -f3 |  sed 's/ //g')
			InstructionsRetiredGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "INSTR_RETIRED_ANY" | cut -d "|" -f4 |  sed 's/ //g')
			ClockUnhaltedGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPU_CLK_UNHALTED_CORE" | cut -d "|" -f4 |  sed 's/ //g')
			ClockUnhaltedRefGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPU_CLK_UNHALTED_REF" | cut -d "|" -f4 |  sed 's/ //g')
			RuntimeUnhaltedGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "INSTR_RETIRED_ANY" | cut -d "|" -f4 |  sed 's/ //g')
			ClockGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "Clock [MHz]" | cut -d "|" -f3 |  sed 's/ //g')
			CPIGen=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPI" | cut -d "|" -f3 |  sed 's/ //g')

			s=$((s+1))
			
			RuntimeCore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "RDTSC Runtime [s]" |  sed 's/ //g')
			CallCountCore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "call count" | cut -d "|" -f3 |  sed 's/ //g')
			InstructionsRetiredCore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "INSTR_RETIRED_ANY" | cut -d "|" -f4 |  sed 's/ //g')
			ClockUnhaltedCore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPU_CLK_UNHALTED_CORE" | cut -d "|" -f4 |  sed 's/ //g')
			ClockUnhaltedRefCore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPU_CLK_UNHALTED_REF" | cut -d "|" -f4 |  sed 's/ //g')
			RuntimeUnhaltedCore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "INSTR_RETIRED_ANY" | cut -d "|" -f4 |  sed 's/ //g')
			ClockCore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "Clock [MHz]" | cut -d "|" -f3 |  sed 's/ //g')
			CPICore=$(cat ${outDir}/${n}.out | grep -n -m ${s} "CPI" | cut -d "|" -f3 |  sed 's/ //g')

			s=$((s+1))

		done

		#Agrupamento
		RuntimeInitTotal=0
		for j in ${RuntimeInit[@]};
		do
			RuntimeInitTotal=$((RuntimeInitTotal+j))
		done

		CallCountInitTotal=0
		for j in ${CallCountInit[@]};
		do
			CallCountInitTotal=$((CallCountInitTotal+j))
		done

		InstructionsRetiredTTotal=0
		for j in ${InstructionsRetiredInit[@]};
		do
			RuntimeTotal=$((RuntimeTotal+j))
		done

				RuntimeTotal=0
		for j in ${RuntimeInit[@]};
		do
			RuntimeTotal=$((RuntimeTotal+j))
		done

				RuntimeTotal=0
		for j in ${RuntimeInit[@]};
		do
			RuntimeTotal=$((RuntimeTotal+j))
		done

				RuntimeTotal=0
		for j in ${RuntimeInit[@]};
		do
			RuntimeTotal=$((RuntimeTotal+j))
		done

				RuntimeTotal=0
		for j in ${RuntimeInit[@]};
		do
			RuntimeTotal=$((RuntimeTotal+j))
		done

				RuntimeTotal=0
		for j in ${RuntimeInit[@]};
		do
			RuntimeTotal=$((RuntimeTotal+j))
		done
	done
done

echo "Init"
			echo "				Runtime $RuntimeInit"
			echo "				Call Count $CallCountInit"
			echo "				Instructions Retired Init $InstructionsRetiredInit"
			echo "				Clock Unhalted $ClockUnhaltedInit"
			echo "				Clock Unhalted Ref $ClockUnhaltedRefInit"
			echo "				Runtime Unhalted $RuntimeUnhaltedInit"
			echo "				Clock $ClockInit"
			echo "				CPI $CPIInit"

echo "Gen"
			echo "				RuntimeGen $RuntimeGen"
			echo "				Call Count $CallCountGen"
			echo "				Instructions Retired $InstructionsRetiredGen"
			echo "				Clock Unhalted $ClockUnhaltedGen"
			echo "				Clock Unhalted Ref $ClockUnhaltedRefGen"
			echo "				Runtime Unhalted $RuntimeUnhaltedGen"
			echo "				Clock $ClockGen"
			echo "				CPI $CPIGen"

echo "Core"
			echo "				RuntimeCore $RuntimeCore"
			echo "				Call Count $CallCountCore"
			echo "				Instructions Retired $InstructionsRetiredCore"
			echo "				Clock Unhalted $ClockUnhaltedCore"
			echo "				Clock Unhalted Ref $ClockUnhaltedRefCore"
			echo "				Runtime Unhalted $RuntimeUnhaltedCore"
			echo "				Clock $ClockCore"
			echo "				CPI $CPICore"