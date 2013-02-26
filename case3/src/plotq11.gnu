#set terminal epslatex solid color colortext  size 15cm,15cm
set terminal pdf solid color 
#size  15cm, 10cm

ddiv(x,y) = x/y
sinc(x) = sin(x)/x


#set xrange [0:5000]
#set yrange [0:100]

set output 'fig_out/q11.pdf'
plot 'fig_out/sincTrain.sorted.dt' using 1:2 with lines lt 1 title 'sinc(x)',\
	'' using 1:(sinc($1)) with lines lt 2 title 'actual sinc(x)
	
set output 'fig_out/norm.sincq11.pdf'
	plot  tanh(x) with lines lt 2 title 'tanh(x)'