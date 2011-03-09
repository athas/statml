#set terminal epslatex solid color colortext  size 15cm,15cm
set terminal pdf solid color size 13.2cm, 15cm

set xrange [ 0.0000 : 480.0000 ] 
set yrange [ 0.0000 :  640.0000] 
set zrange [ 0.0000 :  1] 

set samples 100
set view map
unset surface
set contour base

set size ratio 1.333
set bmargin at screen .1
set tmargin at screen .95
set lmargin at screen .01

set cntrparam bspline
#set cntrparam levels discrete 0.001,20,300, 600
set cntrparam levels auto 10
set isosamples 100

#set key default
set key box
set key at 540,630
#set key out vert

set output 'tex_out/case10.pdf'
set multiplot
##case10


splot 'tex_out/case10.kande1.gnuplot.dat' using 1:2:3:4:5:6 with rgbimage notitle;
splot "tex_out/case10.pdf.gnuplot.dat" using 1:2:3 with lines notitle

unset multiplot

##case11
set output 'tex_out/case11.pdf'
set multiplot
splot 'tex_out/case11.kande2.gnuplot.dat' using 1:2:3:4:5:6 with rgbimage notitle;
splot "tex_out/case11.pdf.gnuplot.dat" using 1:2:3 with lines notitle
unset multiplot
