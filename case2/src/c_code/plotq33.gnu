#set terminal epslatex solid color colortext  size 15cm,15cm
set terminal pdf solid color 
#size  15cm, 10cm

ddiv(x,y) = x/y


set xrange [0:5000]
set yrange [0:100]

set output 'fig_out/q33.pdf'

plot 'fig_out/q33.dat' using 1:2 with lines lt 1 title 'avg. length',\
 '' using 1:3 with lines lt 2 title 'avg. dist',\
 '' using 1:4 with lines lt 3 title '(avg. length)/(avg. dist)'

set output 'fig_out/q33dol.pdf'
set xrange [0:80]
set yrange [0:60]
plot 'fig_out/q33.dat' using 2:3 with lines lt 4 title '(avg. length)/(avg. dist)'
