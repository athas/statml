#include "common_case.h"
#include <stdio.h>
#include <math.h>

/* extent layout:
 * {xmin,xmax,ymin,ymax}*/

#define ABS_PATH "/Volumes/MacOS/Users/drxl/Projects/smml/statml/case2/Data/"

static double M_data[4] ={100.0, 0.0, 0.0,1.0};

static double other_dist(vect* x, vect* z, int lim){
  double wrk[2];
  gsl_matrix_view M = gsl_matrix_view_array(M_data, 2, 2);
  gsl_vector_view x_v = gsl_vector_view_array(x->data, 2);
  gsl_vector_view z_v = gsl_vector_view_array(z->data, 2);
  gsl_vector_view tmp = gsl_vector_view_array(wrk, 2);
  gsl_blas_dgemv(CblasNoTrans, 1, &M.matrix, &x_v.vector, 0, &tmp.vector);
  gsl_blas_dgemv(CblasNoTrans, -1, &M.matrix, &z_v.vector, 1, &tmp.vector);
  return gsl_blas_dnrm2(&tmp.vector);
}

int main(int argv, char** argc){
	printf("\n\nRunning code for question 3.2:\n\n");

  printf("running set C\n");
	mtrx* train_set = read_knoll(ABS_PATH"knollC-train.dt"),
      * test_set  = read_knoll(ABS_PATH"knollC-test.dt");
    
  for (int ik = 1; ik<=9; ik+=2){
    double RMS = knn_knoll_rms(train_set, test_set, ik, other_dist);
    printf("K = %d RMS == %f\n", ik, RMS);
  }
  gsl_matrix_free(train_set);
  gsl_matrix_free(test_set);
}