#!/bin/bash

methods=("bloom" "bloomChained" "bloomNested" "chained" "cuckoo")

for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/flav/mestrado/MHaJoL/methods/lik/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"
	if [ ! -d "$SPLIT_DIR" ]
	then
		mkdir "$SPLIT_DIR"
	fi

	cd ${SPLIT_DIR}
	for m in "${methods[@]}";
	do
		#First split: each execution on a file
		csplit "$OUT_DIR/${m}.out" "/6500U/" "{*}" > /dev/null

		#Rename everything - 00 does not matter - Groups of 7*17 (7 execution for each 17 metrics)
		counter=1
		execution=1
		for file in xx*; 
		do
			sed -i '/Region Info/c\Regio_Info' $file
			
			group=$(cat $file | grep -m 1 "Group 1:" | cut -d ":" -f2 | sed 's/ //g')
		    newName="./${m}_${execution}_${group}"
		    mv $file $newName

		    if [ "${counter}" -gt "6" ]
		    then
		    	counter=1
		    	execution=$((execution+1))
		    else
		    	counter=$((counter+1))
		    fi
		done
	done
	for m in "${methods[@]}";
	do
	 	for file in ${m}_*;
		do
			csplit "$file" "/Region/" "{*}" > /dev/null

			for newfile in xx*; 
			do
				region=$(cat $newfile | grep -m 1 "Region " | cut -d " " -f2 | cut -d "," -f1 | sed 's/ //g')
			    newNewName="${file}_${region}"
			    mv $newfile $newNewName
			done
			rm ${file}
		done
	done
	rm -rf xx*
done

#Last split. To turn everything into csv files, to use spark
for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/flav/mestrado/MHaJoL/methods/lik/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"

	cd ${SPLIT_DIR}

	sequencial=0
	for file in ./*;
	do 
		csplit "$file" "/+-------------------/" "{*}" > /dev/null

		sequencial=0
		for f in xx*;
		do
			mv $f ${file}_${sequencial}
			sequencial=$((sequencial+1))
	
		done
		rm ${file}
	done
#0 1 3 4 6 7 9
done

for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/flav/mestrado/MHaJoL/methods/lik/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"

	cd ${SPLIT_DIR}

	find ${SPLIT_DIR} -type f -name "*0" -exec rm {} +
	find ${SPLIT_DIR} -type f -name "*1" -exec rm {} +
	find ${SPLIT_DIR} -type f -name "*3" -exec rm {} +
	find ${SPLIT_DIR} -type f -name "*4" -exec rm {} +
	find ${SPLIT_DIR} -type f -name "*6" -exec rm {} +
	find ${SPLIT_DIR} -type f -name "*7" -exec rm {} +
	find ${SPLIT_DIR} -type f -name "*9" -exec rm {} +

done


for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/flav/mestrado/MHaJoL/methods/lik/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"

	cd ${SPLIT_DIR}

	for file in *;
	do
		sed -i '/+------------------/d' ${file}
		sed -i 's/ //g' ${file}
		sed -i 's/|/\,/g' ${file}
	done
done