#include "plot.h"
#include <stdio.h>

void init_plot(char* name, char* x_label, char* y_label, plot_type t, plot_header_t* p){
	p->name = name;
	p->x_label = x_label;
	p->y_label = y_label;
	p->type = t;
	p->num_points = 0;
	p->style = "lines";
}

void set_x_y_data(plot_header_t* p, double** data, int num_points){
	p->data = (void*)data;
	p->num_points = num_points;
}

void add_plot(gnuplot_ctrl* ctrl, plot_header_t* p){
	gnuplot_setstyle(ctrl,p->style);

	switch (p->type){
		case STRING_EQ:
			gnuplot_plot_equation(ctrl, (char*)p->data, p->name);
		break;
		case X_Y_RANGE:
		gnuplot_plot_xy(
			ctrl, *((double**)p->data), *((double**)p->data+1),
					p->num_points, p->name);
		default:
		break;
	}
}

static inline gnuplot_ctrl* __plot_common(char* fname, char* format){
	gnuplot_ctrl* g = gnuplot_init();
	char output_str[1024];
	sprintf(output_str, "set terminal %s", format);
	gnuplot_cmd(g, output_str);
	sprintf(output_str, "set output \"%s.%s\"", fname, format);
	gnuplot_cmd(g, output_str);
	return g;
}

void multiplot_out(plot_header_t** p, int num_plots, char* fname, char* format){
	gnuplot_ctrl * g = __plot_common(fname, format);
	for (int i =0; i< num_plots;i++)
		add_plot(g, *(p+i));
	gnuplot_close(g);
}

void plot_out(plot_header_t* p, char* fname, char* format){
	multiplot_out(&p, 1, fname, format);
}


