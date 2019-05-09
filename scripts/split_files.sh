#!/bin/bash
metrics=("L3CACHE" "L2CACHE" "ENERGY" "L2" "L3" "TLB_DATA" "DATA" "ENERGY" "MEM" "BRANCH" "CACHES")

methods=("cuckoo" "vecCuckoo" "ViViD")

if [ -f ./results.csv ]; then
   rm -rf ./results.csv
fi
touch ./results.csv

for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/fscristo/MHaJoL/data/out/joins/_${selectivity}"
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

		# Rename everything - 00 does not matter - Groups of 7*17 (7 execution for each 17 metrics)
		counter=1
		execution=1
		for file in xx*; 
		do
			sed -i '/Region Info/c\Regio_Info' $file
			
			group=$(cat $file | grep -m 1 "Group 1:" | cut -d ":" -f2 | sed 's/ //g')
			sed -i "s/^/${selectivity},${m},${group},/g" $file
		    newName="./${m}_${counter}_${group}_${file}"
		    mv $file $newName
		    
		    counter=$((counter+1))
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
				sed -i "s/^/${region},/g" $newfile
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
	OUT_DIR="/home/fscristo/MHaJoL/data/out/joins/_${selectivity}"
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
	OUT_DIR="/home/fscristo/MHaJoL/data/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"
	cd ${SPLIT_DIR}
	rm *_4 *_7
done

for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/fscristo/MHaJoL/data/out/joins/_${selectivity}"
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
	OUT_DIR="/home/fscristo/MHaJoL/data/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"

	cd ${SPLIT_DIR}

	for file in ${SPLIT_DIR}/*;
	do
		sed -i '/+------------------/d' ${file}
		sed -i 's/ //g' ${file}
		sed -i 's/|/\,/g' ${file}
		sed -i 's/,,/\,/g' $file
		sed -i 's/,PMC0//g' ${file}
		sed -i 's/,PMC1//g' ${file}
		sed -i 's/,PMC2//g' ${file}
		sed -i 's/,PMC3//g' ${file}
		sed -i 's/,FIXC0//g' ${file}
		sed -i 's/,FIXC1//g' ${file}
		sed -i 's/,FIXC2//g' ${file}
		sed -i 's/,FIXC3//g' ${file}
		sed -i 's/,FIXC4//g' ${file}
		sed -i 's/,PWR0//g' ${file}
		sed -i 's/,PWR1//g' ${file}
		sed -i 's/,PWR2//g' ${file}
		sed -i 's/,PWR3//g' ${file}
		sed -i 's/,TMP0//g' ${file}

		cat ${file} | sed 's/.$//' | sed 's/,-/,0/g' &>> /home/fscristo/MHaJoL/data/results.csv
	done
done