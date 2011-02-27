#include "common_case.h"
#include <stdio.h>
#include <math.h>

/* extent layout:
 * {xmin,xmax,ymin,ymax}*/

//#define IMG_IN_DIR "../../../img/"
#define IMG_IN_DIR ""
#define IMG_OUT_DIR "img_out/"

static inline double dmax(double a, double b){
	return a > b ? a : b;}
static inline double dmin(double a, double b){
	return a < b ? a : b;}


pnm_img* img_pdf(pnm_img* img, gsl_vector* sigma, gsl_matrix* cov){
	double one_over = pow(M_PI,3/2) * sqrt(matrix_determinant(cov));
	gsl_matrix* cov_inv = inverse_matrix(cov);
	
	pnm_img* out = pnm_create(img->width, img->height, BINARY_GREYMAP);

	double* map = (double*)malloc(img->width*img->height*sizeof(double));
	double min = 0, max = 0;
	gsl_vector* x = gsl_vector_alloc(3);
	pnm_pixmap* pix = (pnm_pixmap*)img->pixels;
	for (int i =0; i<img->width*img->height; i++,pix++) {
		pix2vec(pix, x);
		gsl_vector_sub(x,sigma);
		*(map+i) = pdf(x, cov_inv)/one_over;
		min = dmin(min,*(map+i));
		max = dmax(max,*(map+i));
	}
	double normalized = 1.0/max;
	for (int i =0; i<img->width*img->height; i++) {
		*((unsigned char*)out->pixels+i) = 0xff*normalized* (*(map+i));
	}
	
	gsl_matrix_free(cov_inv);

	return out;
}

int main(int argv, char** argc){

	int		train_area1[4] = {150-1,330-1,264-1,328-1},
			train_area2[4] = {125-1, 305-1,264-1,328-1};
	
	/*read images from file*/
	pnm_img	* kande1 = pnm_read(IMG_IN_DIR "kande1.pnm"),
			* kande2 = pnm_read(IMG_IN_DIR "kande2.pnm");
	
	pnm_img	* train_img1 =pnm_subimage(kande1, train_area1),
			* train_img2 =pnm_subimage(kande2, train_area2);
	
	gsl_matrix	* train_set1 = img2train_set(train_img1),
				* train_set2 = img2train_set(train_img2);
	
	gsl_vector	* s_mean1 = sample_mean(train_set1),
				* s_mean2 = sample_mean(train_set2);
	
	printf("sample mean:\n");
	print_vec(s_mean1, "|%.5f|\n");
	
	gsl_matrix	* s_cov1 = sample_cov(train_set1, s_mean1),
				* s_cov2 = sample_cov(train_set2, s_mean2);
	
	printf("sample cov:\n");
	print_mtrx(s_cov1, "%.5f |");

	pnm_img	* density1 = img_pdf(kande1, s_mean1, s_cov1),
			*density2 = img_pdf(kande2, s_mean2, s_cov2);

	pnm_write(density1, IMG_OUT_DIR "train_img1.pnm");
	pnm_write(density2, IMG_OUT_DIR "train_img2.pnm");
	
		
	
	gsl_matrix_free(train_set1);
	gsl_matrix_free(train_set2);
	gsl_vector_free(s_mean1);
	gsl_vector_free(s_mean2);
	

}