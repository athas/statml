#include "mycase1.h"

#define NUM_POINTS 500

int main(int argv, char** argc){

	plot_header_t plot;
	char* str_eq = "sin(x)";
	init_plot("sine", "x_l", "y_l", STRING_EQ, &plot);
	plot.data = (void *)str_eq;
	plot_out(&plot, "case1", "svg");
	
	double x[NUM_POINTS];
	double y[NUM_POINTS];
	for (int i=0 ; i<NUM_POINTS; i++) {
		x[i] = (double)(i)/10.0;
		y[i] = x[i] * x[i];
	}
	double* d2[2];
	d2[0] = x;
	d2[1] = y;

	plot_header_t plot2;
	init_plot("parabola", "x_l", "y_l", X_Y_RANGE, &plot2);	
	set_x_y_data(&plot2, d2, NUM_POINTS);
	plot_out(&plot2, "case1.1", "pdf");
}