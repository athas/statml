#include "common_case.h"
#include <stdio.h>
#include <math.h>

/* extent layout:
 * {xmin,xmax,ymin,ymax}*/



int main(int argv, char** argc){
	printf("\n\nRunning code for question 1.11:\n\n");

	int		train_area1[4] = {150-1,330-1,264-1,328-1};

	/*read images from file*/
	pnm_img	* kande1 = pnm_read(IMG_IN_DIR "kande1.pnm"),
			* kande2 = pnm_read(IMG_IN_DIR "kande2.pnm");
	
	pnm_img	* train_img =pnm_subimage(kande1, train_area1);
	mtrx	* train_set = img2train_set(train_img);
	vect	* s_mean = sample_mean(train_set);
	mtrx	* s_cov = sample_cov(train_set, s_mean);
	
	
	double* map = (double*)malloc(kande2->width*kande2->height*sizeof(double));
	pdf_map(kande2, s_mean,s_cov,&map);
	
	vect* w_mean = weighted_avg_pos(kande2, map);
	
	pnm_pixmap p = {0,255,0};
	for (int x = -2; x<4;x++)
		for (int y=-2; y<4; y++) {
			pnm_set_pixel(kande2, (*w_mean->data)+x, *(w_mean->data+1)+y, &p);
		}
		//	pnm_write(kande1, IMG_OUT_DIR"cas10center.pnm");
	
	mtrx* w_cov = weighted_2dcov(map, w_mean, kande2);

	FILE* fp = fopen(TEX_OUT_DIR"c11.tex", "w");
	vect2tex(w_mean, "celevenwmean", fp);
	mtrx2tex(w_cov, "celevenwcov", fp);
	fclose(fp);
	
	gplot_img2splot(kande2, 0 ,TEX_OUT_DIR"case11.kande2.gnuplot.dat");
	gplot_pdf2splot(w_mean, w_cov, kande2, 5, TEX_OUT_DIR"case11.pdf.gnuplot.dat");
	
	gsl_matrix_free(train_set);
	gsl_matrix_free(s_cov);	
	
	gsl_vector_free(s_mean);
	pnm_destroy(kande1);
	pnm_destroy(train_img);
}
