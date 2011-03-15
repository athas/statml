#set terminal epslatex solid color colortext  size 15cm,15cm
set terminal pdf solid color size  15cm, 10cm

ddiv(x,y) = x/y


set xrange [1:1000]
set yrange [0:36]

set output 'fig_out/q33.pdf'
set multiplot

plot 'fig_out/q33.dat' using 1:2 with lines lt 1 title 'mean length'
plot 'fig_out/q33.dat' using 1:3 with lines lt 2 title 'mean dist'
plot 'fig_out/q33.dat' using 1:(ddiv($3,$2)) with lines lt 3 title 'length/dist'

unset multiplot

set output 'fig_out/q33dol.pdf'
set xrange [0:40]
set yrange [0:25]
plot 'fig_out/q33.dat' using 2:3 with lines lt 4 title 'mean of dist'
