#include "common_case.h"

#define NUM_SAMPLES 100
#define oneOverNminusOne 1.0/(NUM_SAMPLES-1.0)

static inline double sample_fun(double rndn, double* covariance, double* mean){
	return rndn*covariance[0]+rndn*covariance[1] + *mean;
}

int main(int argv, char** argc){
	int dim = 2;
	
	double mean[2] = {1.0,1.0};
	double meanML[2];
	
	double covL[4]={0.3, 0.2,
					0.2, 0.2};
	
	gsl_matrix* samples[NUM_SAMPLES];
	
	gsl_matrix_view covL_mv = gsl_matrix_view_array(covL,2,2);
	gsl_matrix_view mean_mv = gsl_matrix_view_array(mean, dim, 1);
	gsl_matrix_view meanML_mv = gsl_matrix_view_array(meanML,dim,1);
	
	gsl_linalg_cholesky_decomp(&(covL_mv.matrix));
	gsl_matrix_set(&covL_mv.matrix, 0, 1, 0.0); //zero the upper triangular part

	gsl_rng * r = gsl_rng_alloc(gsl_rng_taus); //init random, not randomly seeded
	
	gsl_matrix* tmp_mtr = gsl_matrix_alloc(2, 1);
	for (int i = 0; i< NUM_SAMPLES; i++){
		samples[i] = gsl_matrix_alloc(2, 1);
		for (int j=0; j<dim; j++){
			gsl_matrix_set(tmp_mtr, j, 0, gsl_ran_gaussian_ziggurat(r,1));
		}
		//y <- mean + L %*% z
		gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
					   1, &covL_mv.matrix, tmp_mtr,
					   0, samples[i]);
		gsl_matrix_add(samples[i],&mean_mv.matrix);
	}
	
	//compute meanML
	gsl_matrix_set_zero(&meanML_mv.matrix);
	for (int i = 0; i< NUM_SAMPLES; i++){
		gsl_matrix_add(&meanML_mv.matrix,samples[i]);
		printf("meanML: (%f;%f)\n", meanML[0], meanML[1]);
	}
	gsl_matrix_scale(&meanML_mv.matrix, 1.0/NUM_SAMPLES);
	
	double covML[4];
	gsl_matrix_view covML_mv = gsl_matrix_view_array(covML, dim, dim);
	gsl_matrix_set_zero(&covML_mv.matrix);
	for (int i = 0; i< NUM_SAMPLES; i++){
		gsl_matrix_memcpy(tmp_mtr,samples[i]);
		gsl_matrix_sub(tmp_mtr,&meanML_mv.matrix);
		gsl_blas_dgemm(CblasNoTrans, CblasTrans,
						1, tmp_mtr, tmp_mtr,
						1, &covML_mv.matrix);
	}
	gsl_matrix_scale(&covML_mv.matrix, 1.0/(NUM_SAMPLES-1));
	
	double ys[NUM_SAMPLES*2];
	for (int i=0; i<NUM_SAMPLES; i++) {
		ys[i] = *(samples[i]->data);
		ys[i+NUM_SAMPLES] = *(samples[i]->data+1);
	}
	
	graph_header_t plot;
	init_graph(&plot,"Two-dimensional gaussian distribution","pdf");
	plot.x_label = "X";
	plot.y_label ="Y";
	plot_x_y(&plot,mean,mean+1,1,"Actual mean", pt_points);
	plot_x_y(&plot,meanML,meanML+1,1,"Maximum likelyhood mean", pt_points);
	plot_x_y(&plot,ys,ys+NUM_SAMPLES,NUM_SAMPLES,"Random samples sigma=1 mean=T(1,1)", pt_points);

	
	plot.b_box[0] =plot.b_box[2] = -3;
	plot.b_box[1] =plot.b_box[3] = 3;
	
	graph2file(&plot,"case1.3");
}