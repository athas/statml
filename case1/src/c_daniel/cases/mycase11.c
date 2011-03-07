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
#define IMG_IN_DIR "/Volumes/commons/Projects/smml/statml/case1/img/"
#define IMG_OUT_DIR "/Volumes/commons/Downloads/case9-10-11/case9-10-11/build/Debug/img_out/"


static vect* weighted_avg_pos(pnm_img* img, double* map){
	
	double 	X = 0.0,
			Y = 0.0;
	double 	sum = 0.0;
	
	vect* output = gsl_vector_alloc(2);
	for (int iy =0; iy<img->height; iy++){
		for (int ix=0; ix<img->width; ix++){
			int pos = ix+iy*img->width;
			X+= ix*(*(map+pos));
			Y+= iy*(*(map+pos));
			sum+=*(map+pos);
		}
	}
	X /= sum;
	Y /= sum;

	gsl_vector_set(output, 0, X);
	gsl_vector_set(output, 1, Y);
	
	return output;
}

static mtrx* weighted_2dcov(double* weights, double factor, vect* s_mean, pnm_img* img){
	gsl_matrix* tmp_mm = gsl_matrix_alloc(2, 1);
	gsl_matrix* covML = gsl_matrix_alloc(2,2);
	gsl_matrix_set_zero(covML);
	double sum = 0.0;
	for (int iy =0; iy<img->height; iy++){
		for (int ix=0; ix<img->width; ix++){
			int pos = ix+iy*img->width;	
			sum+=weights[pos];
			gsl_matrix_set(tmp_mm, 0, 0, ix - *s_mean->data);
			gsl_matrix_set(tmp_mm, 1, 0, iy -*s_mean->data+1);
			gsl_blas_dgemm(CblasNoTrans,CblasTrans,
						   weights[pos], tmp_mm, tmp_mm,
						   1, covML);
		}
	}
	gsl_matrix_scale(covML, 1.0/sum/factor);
	return covML;
}

static inline int pix2int(pnm_pixmap* p){
	return (p->R <<16) + (p->G <<8) + p->B;
}

void gplot_data2file(vect* mean, pnm_img* img, double* map, char* fname){
	
#define SKIP 1
		//	double* X = (double*)malloc(sizeof(double)*3*npoints);
		//	double	* Y = X+npoints,
		//	* Z = X+2*npoints;
	
	FILE* fp;
	fp = fopen(fname, "w");
	pnm_pixmap* pix = (pnm_pixmap*)img->pixels;

	for (int iy=1; iy<=img->height; iy+=SKIP){
		for (int ix=0; ix<img->width; ix+=SKIP){
			int pos = ix+(img->height-iy)*img->width;
/*			*(X+pos/SKIP) = ix/SKIP;
			*(Y+pos/SKIP) = iy/SKIP;
			*(Z+pos/SKIP) = *(map + pos);
 */
			fprintf(fp, "%d %d %f %d %d %d\n", 
				ix,iy,*(map + pos),
					(pix+pos)->R,(pix+pos)->G,(pix+pos)->B);
					//pix2int(pix+pos));
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
/*	gnuplot_cmd(gc, "set terminal png");
	gnuplot_cmd(gc, "set output \"graph.png\"");
	gnuplot_setstyle(gc,"dots");
	

		//	gnuplot_setstyle(gc,"pm3d");	
		//	gnuplot_splot(gc,X,Y,Z,npoints,"titsdf");

	gnuplot_setstyle(gc,"lines");
	gnuplot_contour_plot(gc,X,Y,Z,img->width/SKIP,img->height/SKIP,"title");
		//	gnuplot_hardcopy_colour(gc,"graph.ps");
	gnuplot_close(gc);*/
		//	free(X);
}



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
	double norm = pdf_map(kande1, s_mean1,s_cov1,&map);

	vect* s_mean = weighted_avg_pos(kande1, map);
	print_vec(s_mean, "<%.3f>\n");

	pnm_pixmap p = {0,0,255};
	for (int x = -2; x<4;x++)
		for (int y=-2; y<4; y++) {
			pnm_set_pixel(kande1, (*s_mean->data)+x, *(s_mean->data+1)+y, &p);
		}
	pnm_write(kande1, IMG_OUT_DIR"cas10center.pnm");
	
	mtrx* s_cov = weighted_2dcov(map, norm, s_mean, kande1);
	print_mtrx(s_cov, " %.3f |");
	
	gplot_data2file(s_mean, kande1, map, "case10.gnuplot.dat");
	
	
	gsl_matrix_free(train_set1);
	gsl_matrix_free(s_cov1);	
	
	gsl_vector_free(s_mean1);
	pnm_destroy(kande1);
	pnm_destroy(train_img1);
}
