#include "common_case.h"

#define NUM_POINTS 1200
#define PLOTS_NUM 3

static int means[PLOTS_NUM] = {-1,2,3};
static int deviations[PLOTS_NUM] = {1,2,3};

static inline double plot_fun(double inp, double mean,double sigma){
	return gsl_ran_gaussian_pdf(inp-mean,sigma);}

static inline double plot1(double inp){
	return plot_fun(inp,means[0],deviations[0]);}
static inline double plot2(double inp){
	return plot_fun(inp, means[1],deviations[1]);}
static inline double plot3(double inp){
	return plot_fun(inp, means[2],deviations[2]);}

int main(int argv, char** argc){
	double(*map_fs[PLOTS_NUM])(double) ={plot1,plot2,plot3};
	char label[PLOTS_NUM][255];

	figure_ctrl plot;
	init_figure(&plot,"Gauss normal","epslatex solid color colortext ");
	plot.x_label = "X";
	plot.y_label ="Y";
	for (int j = 0; j<PLOTS_NUM;j++){
		xy_range_t rxy = range_xy(-8, 14, NUM_POINTS,map_fs[j]);
		snprintf(label[j], sizeof(label), "$\\\\mu$=%d, $\\\\sigma$=%d",means[j],deviations[j]);
		plot_x_y(&plot, rxy.x,rxy.y,NUM_POINTS,label[j], "lines");
	}
//	plot_viewbox(&plot,-1,3,-1,3);
	figure2file(&plot,"case11.tex");
}