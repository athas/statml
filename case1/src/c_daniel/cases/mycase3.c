#include "common_case.h"
#include "plot.h"

#define NUM_SAMPLES 100

static inline double sample_fun(double rndn, double* covariance, double* mean){
	return rndn*covariance[0]+rndn*covariance[1] + *mean;
}

mtrx* generate_samples(int n_samples,vect* mean_mv, mtrx* L){
	int dim = 2;
	//init random, not randomly seeded
	gsl_rng * r = gsl_rng_alloc(gsl_rng_taus); 
	mtrx* smp = gsl_matrix_alloc(n_samples,dim);
	vect* z = gsl_vector_alloc(dim);
	
	for(int i = 0; i< n_samples; i++){
		gsl_vector_view cur =gsl_matrix_row(smp, i);
		gsl_vector_set(z,0,gsl_ran_gaussian_ziggurat(r,1));
		gsl_vector_set(z,1,gsl_ran_gaussian_ziggurat(r,1));
		//y <- mean + L * z
		gsl_blas_dgemv(CblasNoTrans, 
					   1, L, z, 
					   0, &cur.vector);
		gsl_vector_add(&cur.vector, mean_mv);
	}
	gsl_vector_free(z);
	gsl_rng_free(r);
	return smp;
}

int main(int argv, char** argc){
	printf("\n\nRunning code for question 1.3:\n\n");

	double mean[2] = {	1.0,	1.0},
			covL[4]={	0.3, 	0.2,
						0.2, 	0.2};
	gsl_vector_view mean_mv = gsl_vector_view_array(mean,2);	
	gsl_matrix_view L_mv = gsl_matrix_view_array(covL,2,2);
	
	//chol
	gsl_linalg_cholesky_decomp(&L_mv.matrix);
	gsl_matrix_set(&L_mv.matrix, 0, 1, 0.0); //zero the lower triangle
	
	//synthesize samples
	mtrx* samples = generate_samples(NUM_SAMPLES, &mean_mv.vector, &L_mv.matrix);
	
	//calculate most likely mean from samples
	vect* meanML = sample_mean(samples);
	
	printf("sampled mean:\n");
	print_vec(meanML, "|%.5f|");
	
	//sample_cov
	mtrx* covML = sample_cov(samples,&mean_mv.vector);
	printf("sampled covariance matrix:\n");
	print_mtrx(covML,  "%.5f |");
	
	double ys[NUM_SAMPLES*2];
	for (int i=0; i<NUM_SAMPLES; i++) {
		ys[i] = gsl_matrix_get(samples, i, 1);
		ys[i+NUM_SAMPLES] = gsl_matrix_get(samples, i, 0);
	}
	
	figure_ctrl plot;
	init_figure(&plot,"Two-dimensional gaussian distribution","epslatex color colortext rounded");
	plot.x_label = "X";
	plot.y_label ="Y";
	plot_x_y(&plot,ys,ys+NUM_SAMPLES,NUM_SAMPLES,"Generated $\\\\sigma$=1 $\\\\mu=(1,1)^T$", plot_style2str(ps_dots));
	plot_x_y(&plot,mean,mean+1,1,"Actual $\\\\mu$", plot_style2str(ps_points));
	plot_x_y(&plot,meanML->data,meanML->data+1,1,"Sampled  $\\\\mu$",plot_style2str(ps_points));
	
	plot_viewbox(&plot,-1,3,-1,4);
	
	printf("Output plot saved in folder :" TEX_OUT_DIR"\n\n");
	figure2file(&plot,TEX_OUT_DIR"case3.tex");
}