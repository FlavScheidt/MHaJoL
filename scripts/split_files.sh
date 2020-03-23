#!/bin/bash

metrics=("L3CACHE" "L2CACHE" "ENERGY" "MEM" "CACHES")
methods=("avx2" "avx512256" "avx512512" "scalar")


if [ -f /home/flav/Mestrado/MHaJoL/data/results.csv ]; then
   rm -rf /home/flav/Mestrado/MHaJoL/data/results.csv
fi
touch /home/flav/Mestrado/MHaJoL/data/results.csv
echo 'region,selectivity,method,grp,metric,value' >> /home/flav/Mestrado/MHaJoL/data/results.csv

for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/flav/Mestrado/MHaJoL/data/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"
	if [ ! -d "$SPLIT_DIR" ]
	then
		mkdir "$SPLIT_DIR"
	fi
	
	cd ${SPLIT_DIR}
	for m in "${methods[@]}";
	do
		#First split: each execution on a file
		csplit "$OUT_DIR/${m}.out" "/Skylake/" "{*}" > /dev/null

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
	OUT_DIR="/home/flav/Mestrado/MHaJoL/data/out/joins/_${selectivity}"
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
	OUT_DIR="/home/flav/Mestrado/MHaJoL/data/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"
	cd ${SPLIT_DIR}
	rm *_4 *_7
done

for i in `seq 1 9`;
do
	selectivity=$((i*10))
	OUT_DIR="/home/flav/Mestrado/MHaJoL/data/out/joins/_${selectivity}"
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
	OUT_DIR="/home/flav/Mestrado/MHaJoL/data/out/joins/_${selectivity}"
	SPLIT_DIR="${OUT_DIR}/split"

	cd ${SPLIT_DIR}

	for file in ${SPLIT_DIR}/*;
	do
		sed -i '/+------------------/d' ${file}
		sed -i '/nan/d' ${file}
		sed -i 's/\//_/g' ${file}
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

		sed -i 's/,CBOX0C1//g' ${file}
		sed -i 's/,CBOX1C1//g' ${file}
		sed -i 's/,CBOX2C1//g' ${file}
		sed -i 's/,CBOX3C1//g' ${file}
		sed -i 's/,CBOX4C1//g' ${file}
		sed -i 's/,CBOX5C1//g' ${file}
		sed -i 's/,CBOX6C1//g' ${file}
		sed -i 's/,CBOX7C1//g' ${file}
		sed -i 's/,CBOX8C1//g' ${file}
		sed -i 's/,CBOX9C1//g' ${file}
		sed -i 's/,CBOX10C1//g' ${file}
		sed -i 's/,CBOX11C1//g' ${file}
		sed -i 's/,CBOX12C1//g' ${file}
		sed -i 's/,CBOX13C1//g' ${file}
		sed -i 's/,CBOX14C1//g' ${file}
		sed -i 's/,CBOX15C1//g' ${file}
		sed -i 's/,CBOX16C1//g' ${file}
		sed -i 's/,CBOX17C1//g' ${file}
		sed -i 's/,CBOX18C1//g' ${file}
		sed -i 's/,CBOX19C1//g' ${file}
		sed -i 's/,CBOX20C1//g' ${file}
		sed -i 's/,CBOX21C1//g' ${file}
		sed -i 's/,CBOX22C1//g' ${file}
		sed -i 's/,CBOX23C1//g' ${file}
		sed -i 's/,CBOX24C1//g' ${file}
		sed -i 's/,CBOX25C1//g' ${file}
		sed -i 's/,CBOX26C1//g' ${file}
		sed -i 's/,CBOX27C1//g' ${file}
		sed -i 's/,CBOX0C0//g' ${file}
		sed -i 's/,CBOX1C0//g' ${file}
		sed -i 's/,CBOX2C0//g' ${file}
		sed -i 's/,CBOX3C0//g' ${file}
		sed -i 's/,CBOX4C0//g' ${file}
		sed -i 's/,CBOX5C0//g' ${file}
		sed -i 's/,CBOX6C0//g' ${file}
		sed -i 's/,CBOX7C0//g' ${file}
		sed -i 's/,CBOX8C0//g' ${file}
		sed -i 's/,CBOX9C0//g' ${file}
		sed -i 's/,CBOX10C0//g' ${file}
		sed -i 's/,CBOX11C0//g' ${file}
		sed -i 's/,CBOX12C0//g' ${file}
		sed -i 's/,CBOX13C0//g' ${file}
		sed -i 's/,CBOX14C0//g' ${file}
		sed -i 's/,CBOX15C0//g' ${file}
		sed -i 's/,CBOX16C0//g' ${file}
		sed -i 's/,CBOX17C0//g' ${file}
		sed -i 's/,CBOX18C0//g' ${file}
		sed -i 's/,CBOX19C0//g' ${file}
		sed -i 's/,CBOX20C0//g' ${file}
		sed -i 's/,CBOX21C0//g' ${file}
		sed -i 's/,CBOX22C0//g' ${file}
		sed -i 's/,CBOX23C0//g' ${file}
		sed -i 's/,CBOX24C0//g' ${file}
		sed -i 's/,CBOX25C0//g' ${file}
		sed -i 's/,CBOX26C0//g' ${file}
		sed -i 's/,CBOX27C0//g' ${file}
		sed -i 's/,CBOX28C0//g' ${file}
		sed -i 's/,MBOX0C0//g' ${file}
		sed -i 's/,MBOX0C1//g' ${file}
		sed -i 's/,MBOX1C0//g' ${file}
		sed -i 's/,MBOX1C1//g' ${file}
		sed -i 's/,MBOX2C0//g' ${file}
		sed -i 's/,MBOX2C1//g' ${file}
		sed -i 's/,MBOX3C0//g' ${file}
		sed -i 's/,MBOX3C1//g' ${file}
		sed -i 's/,MBOX4C0//g' ${file}
		sed -i 's/,MBOX4C1//g' ${file}
		sed -i 's/,MBOX5C0//g' ${file}
		sed -i 's/,MBOX5C1//g' ${file}

		cat ${file} | sed 's/.$//' | sed 's/,-/,0/g' &>> /home/flav/Mestrado/MHaJoL/data/results.csv
	done
done