#ifndef COMMON_CASE_1_H_GUARD
#define COMMON_CASE_1_H_GUARD
#include <stdio.h>
#include <math.h>
#include "statml.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

#define TEX_OUT_DIR "tex_out/"
#define IMG_IN_DIR ""
#define IMG_OUT_DIR "img_out/"


typedef gsl_vector vect;
typedef gsl_matrix mtrx;

typedef struct {
	int offset;
	double sq_dist;
} keighbor_t;



mtrx *read_knoll(const char file_name[]);

keighbor_t* k_nearest(int k, vect* subj, mtrx* others);

	
#endif /* end of include guard: COMMON_CASE_1_H_GUARD */
