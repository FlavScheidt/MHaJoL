set terminal pngcairo enhanced font 'arial,10' fontscale 1.5 size 1024, 768
set output "./all_joins.png"
unset border
set grid
set style fill  solid 0.25 noborder
set boxwidth 0.75 absolute
set title 'Join Types'
set xlabel  'Selectivity'
set ylabel  'Time (ms)'
set style histogram clustered 
set key under autotitle nobox
set xtics  norangelimit
set xtics   ()
set xtics
plot '/home/flav/mestrado/BloomJoin/gnu/filterBuckets/all/hash.dat' u 2:xtic(1) with histogram title 'Hash Join' axis x1y1, \
'/home/flav/mestrado/BloomJoin/gnu/filterBuckets/all/bloom_nested.dat' u 2:xtic(1) with histogram title 'Bloom + Nested Loop' axis x1y1, \
'/home/flav/mestrado/BloomJoin/gnu/filterBuckets/all/bloom_hash.dat' u 2:xtic(1) with histogram title 'Bloom + Hash Join' axis x1y1