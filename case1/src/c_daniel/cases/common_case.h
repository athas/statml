
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

pnm_img* pnm_subimage(pnm_img* src, int* extent);

gsl_matrix* img2train_set(pnm_img* img);

void pix2vec(pnm_pixmap* pix, gsl_vector* x);

double pdf(gsl_vector* x_m_s, gsl_matrix* cov_inv);

#endif /* end of include guard: MYCASE_1_H_GUARD */
