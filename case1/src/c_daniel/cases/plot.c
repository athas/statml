#undef PLOT_H_CZLG08EY
#include "plot.h"
#include <stdio.h>


char* plot_style2str(plot_style type){
	switch (type) {
		case ps_lines:
			return "lines";
			break;
		case ps_points:
			return "points";
			break;
		case ps_dots:
			return "dots";
			break;
		case ps_steps:
			return "steps";
			break;
		case ps_circles:
			return "circles";
			break;
		case ps_impulses:
			return "impulses";
			break;
		case ps_histeps:
			return "histeps";
			break;
		case ps_linespoints:
			return "linespoints";
			break;
		case ps_boxes:
			return "boxes";
			break;
		case ps_histograms:
			return "histograms";
			break;
			/*		case ps_financebars:
			 return "financebars";
			 break;
			 case ps_xyerrorbars:
			 return "xyerrorbars";
			 break;
			 case ps_xyerrorlines:
			 return "xyerrorlines";
			 break;
			 case ps_yerrorlines:
			 return "yerrorlines";
			 break;
			 case ps_vectors:
			 return "vectors";
			 break;
			 case ps_candlesticks:
			 return "candlesticks";
			 break;
			 case ps_boxxyerrorbars:
			 return "boxxyerrorbars";
			 break;
			 case ps_filledcurves:
			 return "filledcurves";
			 break;
			 case ps_boxerrorbars:
			 return "boxerrorbars";
			 break;
			 case ps_xerrorbar:
			 return "xerrorbar";
			 break;
			 case ps_errorbars:
			 return "errorbars";
			 break;
			 case ps_fsteps:
			 return "fsteps";
			 break;
			 case ps_errorlines:
			 return "errorlines";
			 break;
			 case ps_xerrorlines:
			 return "xerrorlines";
			 break;
			 case ps_yerrorbars:
			 return "yerrorbars";
			 break;
			 case ps_rgbimage:
			 return "rgbimage";
			 break;
			 case ps_rgbalpha:
			 return "rgbalpha";
			 break;
			 case ps_pm3d:
			 return "pm3d";
			 break;
			 case ps_image:
			 return "image";
			 break;
			 case ps_labels:
			 return "labels";
			 break;
			 */
		default:
			return plot_style2str(ps_circles);
			break;
	}
}

inline void raw_gp_cmd(figure_ctrl* p, char* cmd){
	gnuplot_cmd(p->g_ctrl, cmd);
}


void init_figure(figure_ctrl* p, char* name, char* format){
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

void plot_x_y(figure_ctrl* p, double* x, double* y, int num_points, char *label, char* style){
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
	this_p->style = style;
}

void plot_viewbox(figure_ctrl* p, double xmin, double xmax, double ymin, double ymax){
	p->b_box[0]=xmin;
	p->b_box[1]=xmax;
	p->b_box[2]=ymin;
	p->b_box[3]=ymax;
	p->lock_bbox = 1;
}

void figure2file(figure_ctrl* p, char* fname){
	char str_buf[1024];

	gnuplot_ctrl* ctrl = gnuplot_init();
	if (p->x_label)
		gnuplot_set_xlabel (ctrl,p->x_label);
	if (p->y_label)
		gnuplot_set_ylabel (ctrl,p->y_label);
	
	double __ppad = 1.0;
	if (!p->lock_bbox)
		__ppad = 1.1;

	snprintf(str_buf, sizeof(str_buf), "set xrange [%f:%f]",
			 p->b_box[0]<0?p->b_box[0]*__ppad:0,p->b_box[1]*__ppad);
	gnuplot_cmd(ctrl, str_buf);

	snprintf(str_buf, sizeof(str_buf), "set yrange [%f:%f]",
			 p->b_box[2]<0?p->b_box[2]*__ppad:0,p->b_box[3]*__ppad);
	gnuplot_cmd(ctrl, str_buf);
		
	for (int i =0; i<p->num_plots;i++){
		gnuplot_setstyle(ctrl,p->plots[i].style);
		snprintf(str_buf, sizeof(str_buf), "set terminal %s", p->format);
		gnuplot_cmd(ctrl, str_buf);
		
		snprintf(str_buf, sizeof(str_buf), "set output \"%s\"", fname);
		gnuplot_cmd(ctrl, str_buf);
		gnuplot_plot_xy(ctrl, p->plots[i].x, p->plots[i].y, p->plots[i].num_points, p->plots[i].label);
	}
	gnuplot_close(ctrl);
}
