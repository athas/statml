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

#define DATA_PATH "../Data/"
#define FIG_OUT "../report/figs/"

typedef gsl_vector vect;
typedef gsl_matrix mtrx;


int file2mtrx(const char file_name[], mtrx* trgt);
void mtrx2file (mtrx* coords, char* fname);

#endif /* end of include guard: COMMON_CASE_1_H_GUARD */
