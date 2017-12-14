set terminal pngcairo enhanced font 'arial,10' fontscale 1.5 size 1024, 768
set output '/home/flav/mestrado/BloomJoin/gnu/joinTime.png'
unset border
set grid
set style fill  solid 0.25 noborder
set boxwidth 0.5 absolute
set xtics rotate
set title 'Join Method'
set xlabel  'Join Method'
set ylabel  'Time (ms) '
set style histogram errorbars gap 2 lw 1
set style data histograms
set xrange [-0.5:]
set key under autotitle nobox
plot '//home/flav/mestrado/BloomJoin/gnu/joinTime.dat' using 0:1 fc rgb 'forest-green' title 'Time (ms) '
