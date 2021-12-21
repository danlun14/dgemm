set terminal png size 1600, 900 font 'Verdana, 10'
set output "result.png"
set title "dgemm"
set xlabel 'N'
set ylabel 'Time(sec)'
set xzeroaxis lt -1
set yzeroaxis lt -1
set key left top


set grid xtics lc rgb  '#555555' lw 1 lt 0
set grid ytics lc rgb  '#555555' lw 1 lt 0

set xtics axis
set ytics axis
plot 'dgemm.txt' using 1:2 with linespoints lw 1 lt rgb 'red' title 'dgemm def', \
 'dgemm_t.txt' using 1:2 with linespoints lw 1 lt rgb 'yellow' title 'dgemm transpose', \
 'dgemm_b.txt' using 1:2 with linespoints lw 1 lt rgb 'blue' title 'dgemm blocks', \
 'dgemm_avx.txt' using 1:2 with linespoints lw 1 lt rgb 'green' title 'dgemm transpose avx'
