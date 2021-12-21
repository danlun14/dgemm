set terminal png size 1000, 450 font 'Verdana, 10'
set output 'bloksize.png' 
set title "Blocksize impact"
set xlabel 'Time(sec)'
set ylabel 'Blocksize'
set xzeroaxis lt -1
set yzeroaxis lt -1
set key left top

set grid xtics lc rgb  '#555555' lw 1 lt 0
set grid ytics lc rgb  '#555555' lw 1 lt 0

set xtics axis
set ytics axis
plot 'dgemm_b_s.txt' using 1:2 with linespoints lw 1 lt rgb 'blue' title 'block size time'
