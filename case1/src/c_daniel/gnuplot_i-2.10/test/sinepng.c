#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gnuplot_i.h"

int main(int argc, char * argv[])
{
	gnuplot_ctrl * g = gnuplot_init();
	
	char out_cmd[1024];
	
	gnuplot_cmd(g, "set terminal png");
	gnuplot_cmd(g, "set output \"sine.png\"");
	gnuplot_plot_equation(g, "sin(x)", "Sine wave");
	gnuplot_close(g);

	return 0 ;
}

