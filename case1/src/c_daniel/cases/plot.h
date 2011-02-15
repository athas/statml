#ifndef PLOT_H_CZLG08EY
#define PLOT_H_CZLG08EY

#include "gnuplot_i.h"

#endif /* end of include guard: PLOT_H_CZLG08EY */

typedef enum {
	STRING_EQ,
	X_Y_RANGE,
	Y_RANGE
} plot_type;

typedef struct {
	char* name;
	char* x_label;
	char* y_label;
	char* style;
	plot_type type;
	void* data;
	int num_points;
} plot_header_t;


void plot_out(plot_header_t*, char*, char*);

void init_plot(char*, char*, char*, plot_type, plot_header_t*);

void set_x_y_data(plot_header_t* p, double** data, int num_points);