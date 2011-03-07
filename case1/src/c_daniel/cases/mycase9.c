#include "ppm.h"
#include "common_case.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


/* extent layout:
 * {xmin,xmax,ymin,ymax}*/

//#define IMG_IN_DIR "../../../img/"
#define IMG_IN_DIR "img_in/"
#define IMG_OUT_DIR "img_out/"

static inline double dmax(double a, double b){
	return a > b ? a : b;}
static inline double dmin(double a, double b){
	return a < b ? a : b;}


int main(int argv, char** argc){
	printf("\n\nRunning code for question 1.9:\n\n");

	int		train_area1[4] = {150-1,330-1,264-1,328-1},
			train_area2[4] = {125-1, 305-1,264-1,328-1};
	
	/*read images from file*/
	pnm_img	* kande1 = pnm_read(IMG_IN_DIR "kande1.pnm"),
			* kande2 = pnm_read(IMG_IN_DIR "kande2.pnm");
	
	pnm_img	* train_img1 =pnm_subimage(kande1, train_area1),
			* train_img2 =pnm_subimage(kande2, train_area2);
	
	mtrx	* train_set1 = img2train_set(train_img1),
			* train_set2 = img2train_set(train_img2);
	
	vect	* s_mean1 = sample_mean(train_set1),
			* s_mean2 = sample_mean(train_set2);
		
	mtrx	* s_cov1 = sample_cov(train_set1, s_mean1),
			* s_cov2 = sample_cov(train_set2, s_mean2);
	
	pnm_img	* density1 = img_pdf(kande1, s_mean1, s_cov1),
			*density2 = img_pdf(kande2, s_mean2, s_cov2);

	mtrx	* ttrain_set1 = refined_img2train(train_img1, s_mean1, s_cov1);
	vect	* tt_mean1 = sample_mean(ttrain_set1);
	mtrx	* tt_cov1 = sample_cov(ttrain_set1, tt_mean1);
	pnm_img	* density3 = img_pdf(kande1, tt_mean1, tt_cov1);
	pnm_write(density3, IMG_OUT_DIR"tt_res1.pnm");
	
	pnm_write(train_img1, IMG_OUT_DIR"train_set1.pnm");
	
	printf("Output images saved in folder :" IMG_OUT_DIR"\n\n");
	pnm_write(density1, IMG_OUT_DIR "train_img1.pnm");
	pnm_write(density2, IMG_OUT_DIR "train_img2.pnm");
	
	printf("sample mean1:\n");
	print_vec(s_mean1, "|%.5f|\n");
	printf("sample cov1:\n");
	print_mtrx(s_cov1, "%.5f |");
	
	gsl_matrix_free(train_set1);
	gsl_matrix_free(train_set2);
	gsl_matrix_free(s_cov1);	
	gsl_matrix_free(s_cov2);
	gsl_matrix_free(ttrain_set1);
	
	
	gsl_vector_free(s_mean1);
	gsl_vector_free(s_mean2);
	pnm_destroy(kande1);
	pnm_destroy(kande2);
	pnm_destroy(train_img1);
	pnm_destroy(train_img2);
	pnm_destroy(density1);
	pnm_destroy(density2);
	pnm_destroy(density3);
}