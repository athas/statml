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

int main(int argv, char** argc){
  mtrx* data = gsl_matrix_alloc(50, 2);
  file2mtrx("../Data/sincTrain50.dt", data);
  
  print_mtrx(data);
  
  mtrx2file(data, "../Data/sincTrain50_out.dt");
}
