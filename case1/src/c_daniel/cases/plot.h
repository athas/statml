#ifndef PLOT_H_CZLG08EY
#define PLOT_H_CZLG08EY

#include "gnuplot_i.h"

#define MAX_PLOTS 20

#ifdef __cplusplus
extern "C" {
#endif
	
typedef enum{
	ps_lines,
	ps_points,
	ps_dots,
	ps_steps,
	ps_circles,
	ps_impulses,
	ps_histeps,
	ps_linespoints,
	ps_boxes,
	ps_histograms
/*, the following are intentionally left out
	ps_financebars,
	ps_xyerrorbars,
	ps_xyerrorlines,
	ps_yerrorlines,
	ps_vectors,
	ps_candlesticks,
	ps_boxxyerrorbars,
	ps_filledcurves,
	ps_boxerrorbars,
	ps_xerrorbar,
	ps_errorbars,
	ps_fsteps,
	ps_errorlines,
	ps_xerrorlines,
	ps_yerrorbars,
	ps_rgbimage,
	ps_rgbalpha,
	ps_pm3d,
	ps_image,
	ps_labels*/
} plot_style;


typedef struct{
	char* style;
	char* label;
	double* x;
	double* y;
	int num_points;
} plot_data_t;

typedef struct {
	char* name;
	char* format;	
	char* x_label;
	char* y_label;
	char lock_bbox;
	double b_box[4];
	gnuplot_ctrl* g_ctrl;
	int num_plots;
	plot_data_t* plots;
} figure_ctrl;

void raw_gp_cmd(figure_ctrl* p, char* cmd);

char* plot_style2str(plot_style type);

void plot_viewbox(figure_ctrl* p, double xmin, double xmax, double ymin, double ymax);

void init_figure(figure_ctrl* p, char* name,char* format);

void plot_x_y(figure_ctrl* p, double* x, double* y, int num_points, char *label, char* style);

void figure2file(figure_ctrl* p,char* fname);
#ifdef __cplusplus
}
#endif
	
#endif /* end of include guard: PLOT_H_CZLG08EY */




