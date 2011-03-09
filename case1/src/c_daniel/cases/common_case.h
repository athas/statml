#ifndef MYCASE_1_H_GUARD
#define MYCASE_1_H_GUARD
#include <stdio.h>
#include <math.h>
#include "ppm.h"
#include "plot.h"
#include "statml/statml.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

#define TEX_OUT_DIR "tex_out/"
#define IMG_IN_DIR "img_in/"
#define IMG_OUT_DIR "img_out/"


typedef gsl_vector vect;
typedef gsl_matrix mtrx;


pnm_img* pnm_subimage(pnm_img* src, int* extent);

gsl_matrix* img2train_set(pnm_img* img);

void pix2vec(pnm_pixmap* pix, gsl_vector* x);

double pdf(gsl_vector* x_m_s, gsl_matrix* cov_inv, gsl_matrix* tmp_m);

double pdf_map(pnm_img* img, vect* sigma, mtrx* cov, double** map);

pnm_img* img_pdf(pnm_img* img, vect* sigma, mtrx* cov);

mtrx* refined_img2train(pnm_img* img, vect* sigma, mtrx* cov);

void gplot_img2splot(pnm_img* img, double Z, char* fname);

void gplot_pdf2splot(vect* mean, mtrx* cov, pnm_img* img, int skip, char* fname);

vect* weighted_avg_pos(pnm_img* img, double* map);

mtrx* weighted_2dcov(double* weights, vect* s_mean, pnm_img* img);

	
#endif /* end of include guard: MYCASE_1_H_GUARD */
