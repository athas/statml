#ifndef PLOT_H_CZLG08EY
#define PLOT_H_CZLG08EY

#include "gnuplot_i.h"

#define MAX_PLOTS 20


typedef enum{
	pt_lines,
	pt_points,
	pt_linespoints,
	pt_impulses,
	pt_dots,
	pt_steps,
	pt_errorbars,
	pt_boxes,
	pt_boxeserrorbars
} pt_enum;


typedef struct{
	char* label;
	pt_enum style;
	double* x;
	double* y;
	int num_points;
} plot_data_t;

typedef struct {
	char* name;
	char* format;	
	char* x_label;
	char* y_label;
	double b_box[4];
	gnuplot_ctrl* g_ctrl;
	int num_plots;
	plot_data_t* plots;
} graph_header_t;


typedef struct{
	double* x;
	double* y;
} xy_range_t;

void set_range(graph_header_t* p, char axis, double from, double to);

void init_graph(graph_header_t* p, char* name,char* format);

void plot_x_y(graph_header_t* p, double* x, double* y, int num_points, char *label, pt_enum);

void graph2file(graph_header_t* p,char* fname);

double* range_x(double, double, int);

xy_range_t range_xy(double, double, int, double (*)(double));

#endif /* end of include guard: PLOT_H_CZLG08EY */




