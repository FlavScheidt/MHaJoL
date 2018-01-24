#!/bin/bash

OUT_DIR="/home/flav/mestrado/BloomJoin/gnu"
if [ ! -d "$OUT_DIR" ]
then
	mkdir "$OUT_DIR"
fi


#Selectivity from 0 to 1.0, by 0.1
for i in `seq 1 9`;
do
	selectivity=$((i*10))

	DAT_DIR="${OUT_DIR}/filterBuckets/new/_${selectivity}"
	OUTPUT_FILE="${OUT_DIR}/filterBuckets_${i}.gnu"
	OUTPUT_IMAGE="${OUT_DIR}/filterBuckets_${i}.png"

	echo "set terminal pngcairo enhanced font 'arial,10' fontscale 1.5 size 1024, 768" &>> ${OUTPUT_FILE}
	echo "set output '${OUTPUT_IMAGE}'" &>> ${OUTPUT_FILE}
	echo "unset border" &>> ${OUTPUT_FILE}
	echo "set grid" &>> ${OUTPUT_FILE}
	echo "set style fill  solid 0.25 noborder" &>> ${OUTPUT_FILE}
	echo "set boxwidth 0.75 absolute" &>> ${OUTPUT_FILE}
	echo "set title 'Filter Buckets vs Precision ${selectivity}% selectivity'" &>> ${OUTPUT_FILE}
	echo "set xlabel  '# Buckets'" &>> ${OUTPUT_FILE}
	echo "set ylabel  '% Registers'" &>> ${OUTPUT_FILE}
	echo "set y2label 'Time (ms)'" &>> ${OUTPUT_FILE}
	echo "set style histogram clustered " &>> ${OUTPUT_FILE}
	echo "set style line 5 lt rgb 'cyan' lw 3 pt 6" &>> ${OUTPUT_FILE}
	echo "set key under autotitle nobox" &>> ${OUTPUT_FILE}
	echo "set yrange [0:105]" &>> ${OUTPUT_FILE}
	echo "set xtics  norangelimit" &>> ${OUTPUT_FILE}
	echo "set xtics   ()" &>> ${OUTPUT_FILE}
	echo "set xtics" &>> ${OUTPUT_FILE}
	echo "set x2range [0:10]" &>> ${OUTPUT_FILE}
	#echo "set x2tics" &>> ${OUTPUT_FILE}
	echo "set y2tics" &>> ${OUTPUT_FILE}
	echo "set y2tics ()" &>> ${OUTPUT_FILE}
	echo "set y2range [0:1000]" &>> ${OUTPUT_FILE}
	echo -n "plot '${DAT_DIR}/_0.dat' u 3:xtic(1) with histogram title '1 Hash Function' axis x1y1," &>> ${OUTPUT_FILE}
	echo -n "'${DAT_DIR}/_1.dat' u 3:xtic(1) with histogram title '2 Hash Functions' axis x1y1," &>> ${OUTPUT_FILE}
	echo -n "'${DAT_DIR}/_2.dat' u 3:xtic(1) with histogram title '3 Hash Functions' axis x1y1," &>> ${OUTPUT_FILE}
	echo -n "'${DAT_DIR}/_3.dat' u 3:xtic(1) with histogram title '4 Hash Functions' axis x1y1," &>> ${OUTPUT_FILE}
	echo -n "'${DAT_DIR}/_4.dat' u 3:xtic(1) with histogram title '5 Hash Functions' axis x1y1," &>> ${OUTPUT_FILE}
	echo -n "'${DAT_DIR}/time.dat' with linespoints title 'Time' axis x2y2" &>> ${OUTPUT_FILE}

	gnuplot ${OUTPUT_FILE}
done