set terminal epslatex solid color colortext
set output 'tex_out/case10.tex'

#set terminal pdf solid color
#set output 'case10.gnuplot.pdf'

set xrange [ 0.0000 : 480.0000 ] 
set yrange [ 0.0000 :  640.0000] 
set multiplot

set samples 100
set view map

set size ratio 1.333
set bmargin at screen .1
set tmargin at screen .95
set lmargin at screen .01

set cntrparam bspline
set cntrparam levels discrete 0.001,20,300, 600
set isosamples 100
unset surface
set contour base

set key default
set key box
set key out vert

set origin -0.09,0
#case10
set output 'tex_out/case10.tex'
splot 'tex_out/case10.gnuplot.dat' using 1:2:3:4:5:6 with rgbimage notitle;
set origin 0,0
splot "tex_out/case10.gnuplot.dat" using 1:2:3 with lines notitle

#case11
set output 'tex_out/case10.tex'
splot 'tex_out/case11.gnuplot.dat' using 1:2:3:4:5:6 with rgbimage notitle;
set origin 0,0
splot "tex_out/case11.gnuplot.dat" using 1:2:3 with lines notitle




unset multiplot