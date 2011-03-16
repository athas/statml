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

#define DATA_PATH "../../Data/"
#define FIG_OUT "fig_out/"

typedef gsl_vector vect;
typedef gsl_matrix mtrx;

typedef struct {
	int offset;
  int value;
	double sq_dist;
} keighbor_t;


typedef double(dist_fun_t)(vect*,vect*,int);

mtrx *read_knoll(const char file_name[]);

void k_nearest(int k, vect* test_subj, mtrx* cohorte,keighbor_t* output, 
               dist_fun_t dist_fun_pointer);

double knn_knoll_hitrate(mtrx* train, mtrx* test, int K, dist_fun_t);

void gplot_knn2plot(mtrx* coords, char* fname);

	
#endif /* end of include guard: COMMON_CASE_1_H_GUARD */
