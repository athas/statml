#include "plot.h"
#include <stdio.h>


double* range_x(double from, double to, int num_points){
	double incr = (to-from)/num_points;
	double tmpx=from;
	double* points = (double*)malloc(sizeof(double)*num_points);
	for (int i=0; i<num_points; i++,tmpx+=incr)
		points[i] = tmpx;
	return points;
}

xy_range_t range_xy(double from, double to, int num_points,double(*map_fun)(double)){
	xy_range_t out;
	out.x = range_x(from, to, num_points);
	out.y = (double*)malloc(sizeof(double)*num_points);
	for (int i=0; i<num_points; i++)
		out.y[i] = map_fun(out.x[i]);
	return out;
}


void init_graph(graph_header_t* p, char* name, char* format){
	p->g_ctrl = gnuplot_init();
	p->num_plots = 0;
	p->name = name;
	p->format = format;
	
	p->name = name;
	//p->b_box[1]=p->b_box[2]=p->b_box[3]=0.0;
	p->x_label = NULL;
	p->y_label = NULL;
	for (int i =0; i<4; i++){
		p->b_box[i]=0;
	}
	p->plots = (plot_data_t*)malloc(sizeof(plot_data_t)*MAX_PLOTS);
}

static inline double dmax(double a, double b){
	return a > b ? a : b;}
static inline double dmin(double a, double b){
	return a < b ? a : b;}

static inline char* type2char(pt_enum type){
	switch (type) {
	case pt_points:
		return "points";
		break;
	case pt_linespoints:
		return "linespoints";
		break;
	case pt_impulses:
		return "impulses";
		break;
	case pt_dots:
		return "dots";
		break;
	case pt_steps:
		return "steps";
		break;
	case pt_errorbars:
		return "errorbars";
		break;
	case pt_boxes:
		return "boxes";
		break;
	case pt_boxeserrorbars:
		return "boxeserrorbars";
		break;
	default:
		return "lines";
		break;
	}
}

void plot_x_y(graph_header_t* p, double* x, double* y, int num_points, char *label, pt_enum type){
	p->b_box[0] = dmin(p->b_box[0], x[0]);
	p->b_box[1] = dmax(p->b_box[1], x[num_points-1]);

	double ymin=p->b_box[2];
	double ymax=p->b_box[3];
	for (int i = 0; i<num_points; i++) {
		ymin = dmin(ymin,y[i]);
		ymax = dmax(ymax, y[i]);
	}
	p->b_box[2] = ymin;
	p->b_box[3] = ymax;

	plot_data_t* this_p =  p->plots+p->num_plots++;
	this_p->num_points = num_points;
	this_p->x = x;
	this_p->y = y;
	this_p->label=label;
	this_p->style = type;
}

void set_range(graph_header_t* p, char axis, double from, double to){
	
}

void graph2file(graph_header_t* p, char* fname){
	char str_buf[1024];

	gnuplot_ctrl* ctrl = gnuplot_init();
	if (p->x_label)
		gnuplot_set_xlabel (ctrl,p->x_label);
	if (p->y_label)
		gnuplot_set_ylabel (ctrl,p->y_label);
	
#define PPAD_F 1.1
	snprintf(str_buf, sizeof(str_buf), "set yrange [%f:%f]",
			 p->b_box[2]<0?p->b_box[2]*PPAD_F:0,p->b_box[3]*PPAD_F);
	gnuplot_cmd(ctrl, str_buf);

	snprintf(str_buf, sizeof(str_buf), "set xrange [%f:%f]",
			 p->b_box[2]<0?p->b_box[0]*PPAD_F:0,p->b_box[1]*PPAD_F);
	gnuplot_cmd(ctrl, str_buf);
	
	
	for (int i =0; i<p->num_plots;i++){
		gnuplot_setstyle(ctrl,type2char(p->plots[i].style));
		snprintf(str_buf, sizeof(str_buf), "set terminal %s", p->format);
		gnuplot_cmd(ctrl, str_buf);
		
		snprintf(str_buf, sizeof(str_buf), "set output \"%s.%s\"", fname, p->format);
		gnuplot_cmd(ctrl, str_buf);
		gnuplot_plot_xy(ctrl, p->plots[i].x, p->plots[i].y, p->plots[i].num_points, p->plots[i].label);
	}
	gnuplot_close(ctrl);
}

