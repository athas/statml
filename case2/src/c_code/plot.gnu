#set terminal epslatex solid color colortext  size 15cm,15cm
set terminal pdf solid color size 10cm, 10cm

set xrange [-3:3]
set yrange [-3:3]
ry(v) = v==1?16711680:16776960
bb(v) = v==1?0:255


set output 'fig_out/setA.pdf'
set multiplot
plot '../../Data/knollA-test.dt.ln' using 1:2:(ry($3)) with points pt 6 ps 1 lc rgb variable, \
'' using 1:2:(sprintf("%d",$4)) with labels

plot '../../Data/knollA-train.dt' using 1:2:(bb($3)) with points pt 6 ps 1 lc rgb variable
unset multiplot

set output 'fig_out/setB.pdf'
set multiplot
plot '../../Data/knollB-test.dt' using 1:2:(ry($3)) with points pt 6 ps 1 lc rgb variable
#'' using 1:2:(sprintf("%d",$4)) with labels

plot '../../Data/knollB-train.dt' using 1:2:(bb($3)) with points pt 6 ps 1 lc rgb variable
unset multiplot

set output 'fig_out/setC.pdf'
set multiplot
plot '../../Data/knollC-test.dt' using 1:2:(ry($3)) with points pt 6 ps 1 lc rgb variable
#'' using 1:2:(sprintf("%d",$4)) with labels

plot '../../Data/knollC-train.dt' using 1:2:(bb($3)) with points pt 6 ps 1 lc rgb variable
unset multiplot


set xrange [1:100]
set yrange [0:20]

set output 'fig_out/q33.pdf'
set multiplot
plot 'q33.dat' using 1:2 with lines
plot 'q33.dat' using 1:3 with lines 
unset multiplot
