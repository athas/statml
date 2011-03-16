#include "common_case.h"
#include <stdio.h>
#include <math.h>

/* extent layout:
 * {xmin,xmax,ymin,ymax}*/

static double sq_dist(vect* v1, vect* v2, int lim){
  double  sum = 0,
  tmp;
  
	for(int i= 0; i<lim; i++){
    tmp = *(v1->data+i) - *(v2->data+i);
		sum+= tmp*tmp;
	}
	return sum;
}

static double M_data[4] ={100.0, 0.0, 0.0,1.0};

static double other_dist(vect* x, vect* z, int lim){
  gsl_matrix_view M = gsl_matrix_view_array(M_data, 2, 2);
  double wrk[2];
  gsl_vector_view tmp = gsl_vector_view_array(wrk, 2);
  gsl_blas_dgemv(CblasNoTrans, 1, &M.matrix, x, 0, &tmp.vector);
  gsl_blas_dgemv(CblasNoTrans, -1, &M.matrix, x, 1, &tmp.vector);
  return gsl_blas_dnrm2(&tmp.vector);
}

int main(int argv, char** argc){
	printf("\n\nRunning code for question 3.1:\n\n");

  printf("running set A\n");
	mtrx* train_set = read_knoll(DATA_PATH"knollA-train.dt"),
      * test_set  = read_knoll(DATA_PATH"knollA-test.dt");
    
  for (int ik = 1; ik<=9; ik+=2){
    double RMS = knn_knoll_hitrate(train_set, test_set, ik, sq_dist);
    printf("K = %d hitrate == %.1f%%\n", ik, RMS*100);
  }
  gsl_matrix_free(train_set);
  gsl_matrix_free(test_set);
  
  printf("running set B\n");
  train_set = read_knoll(DATA_PATH"knollB-train.dt"),
  test_set  = read_knoll(DATA_PATH"knollB-test.dt");
  for (int ik = 1; ik<=9; ik+=2){
    double RMS = knn_knoll_hitrate(train_set, test_set, ik,sq_dist);
    printf("K = %d hitrate == %.1f%%\n", ik, RMS*100);
  }
  gsl_matrix_free(train_set);
  gsl_matrix_free(test_set);
  
  printf("running set C\n");
  train_set = read_knoll(DATA_PATH"knollC-train.dt"),
  test_set  = read_knoll(DATA_PATH"knollC-test.dt");
  for (int ik = 1; ik<=9; ik+=2){
    double RMS = knn_knoll_hitrate(train_set, test_set, ik,sq_dist);
    printf("K = %d hitrate == %.1f%%\n", ik, RMS*100);
  }
  gsl_matrix_free(train_set);
  gsl_matrix_free(test_set);
}
