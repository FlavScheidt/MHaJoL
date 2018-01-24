set terminal pngcairo enhanced font 'arial,10' fontscale 1.5 size 1024, 768
set output '/home/flav/mestrado/BloomJoin/gnu/filterBuckets_8.png'
unset border
set grid
set style fill  solid 0.25 noborder
set boxwidth 0.75 absolute
set title 'Filter Buckets vs Precision 80% selectivity'
set xlabel  '# Buckets'
set ylabel  '% Registers'
set y2label 'Time (ms)'
set style histogram clustered 
set style line 5 lt rgb 'cyan' lw 3 pt 6
set key under autotitle nobox
set yrange [0:105]
set xtics  norangelimit
set xtics   ()
set xtics
set x2range [0:10]
set y2tics
set y2tics ()
set y2range [0:1000]
plot '/home/flav/mestrado/BloomJoin/gnu/filterBuckets/new/_80/_0.dat' u 3:xtic(1) with histogram title '1 Hash Function' axis x1y1,'/home/flav/mestrado/BloomJoin/gnu/filterBuckets/new/_80/_1.dat' u 3:xtic(1) with histogram title '2 Hash Functions' axis x1y1,'/home/flav/mestrado/BloomJoin/gnu/filterBuckets/new/_80/_2.dat' u 3:xtic(1) with histogram title '3 Hash Functions' axis x1y1,'/home/flav/mestrado/BloomJoin/gnu/filterBuckets/new/_80/_3.dat' u 3:xtic(1) with histogram title '4 Hash Functions' axis x1y1,'/home/flav/mestrado/BloomJoin/gnu/filterBuckets/new/_80/_4.dat' u 3:xtic(1) with histogram title '5 Hash Functions' axis x1y1,'/home/flav/mestrado/BloomJoin/gnu/filterBuckets/new/_80/time.dat' with linespoints title 'Time' axis x2y2