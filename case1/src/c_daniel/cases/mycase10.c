#include "ppm.h"
#include "common_case.h"
#include <stdio.h>
#include <math.h>


int main(int argv, char** argc){
	printf("\n\nRunning code for question 1.10:\n\n");
	
	int		train_area1[4] = {150-1,330-1,264-1,328-1};
	
	/*read images from file*/
	pnm_img	* kande1 = pnm_read(IMG_IN_DIR "kande1.pnm");
	
	pnm_img	* train_img1 =pnm_subimage(kande1, train_area1);
	mtrx	* train_set1 = img2train_set(train_img1);
	vect	* s_mean1 = sample_mean(train_set1);
	mtrx	* s_cov1 = sample_cov(train_set1, s_mean1);
	
	double* map = (double*)malloc(kande1->width*kande1->height*sizeof(double));
	pdf_map(kande1, s_mean1,s_cov1,&map);

	vect* w_mean = weighted_avg_pos(kande1, map);
	
	pnm_pixmap p = {0,255,0};
	for (int x = -2; x<4;x++)
		for (int y=-2; y<4; y++) {
			pnm_set_pixel(kande1, (*w_mean->data)+x, *(w_mean->data+1)+y, &p);
		}
		//	pnm_write(kande1, IMG_OUT_DIR"cas10center.pnm");
	
	mtrx* w_cov = weighted_2dcov(map, w_mean, kande1);
	
	FILE* fp = fopen(TEX_OUT_DIR"c10.tex", "w");
	vect2tex(s_mean1, "ctensmean", fp);
	mtrx2tex(s_cov1, "ctenscov", fp);	
	vect2tex(w_mean, "ctenwmean", fp);
	mtrx2tex(w_cov, "ctenwcov", fp);
	fclose(fp);
	
	gplot_img2splot(kande1, 0 ,TEX_OUT_DIR"case10.kande1.gnuplot.dat");
	gplot_pdf2splot(w_mean, w_cov, kande1, 5, TEX_OUT_DIR"case10.pdf.gnuplot.dat");

	gsl_matrix_free(train_set1);
	gsl_matrix_free(s_cov1);	
	
	gsl_vector_free(s_mean1);
	pnm_destroy(kande1);
	pnm_destroy(train_img1);
}
