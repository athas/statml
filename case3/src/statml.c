#include "statml.h"
#include "math.h"

double* range_x(double from, double to, int num_points){
	double incr = (to-from)/num_points;
	double tmpx=from;
	double* points = (double*)malloc(sizeof(double)*num_points);
	for (int i=0; i<num_points; i++,tmpx+=incr)
		points[i] = tmpx;
	return points;
}

xy_range_t range_xy(double from, double to, int num_points,double(*map_fun)(double)){
	xy_range_t out;
	out.x = range_x(from, to, num_points);
	out.y = (double*)malloc(sizeof(double)*num_points);
	for (int i=0; i<num_points; i++)
		out.y[i] = map_fun(out.x[i]);
	return out;
}


//compute most likely mean
gsl_vector* sample_mean(gsl_matrix* samples){
	gsl_vector* meanML = gsl_vector_alloc(samples->size2);
	gsl_vector_set_zero(meanML);
	

	for (int i = 0; i< samples->size1; i++){
		gsl_vector_view cur = gsl_matrix_row(samples,i);
		gsl_vector_add(meanML,&cur.vector);
	}
	gsl_vector_scale(meanML, 1.0/samples->size1);
	return meanML;
}

gsl_matrix* sample_cov(gsl_matrix* samples, gsl_vector* meanML){
	gsl_vector* tmp_mtr = gsl_vector_alloc(samples->size2);
	gsl_matrix_view tmp_mm = gsl_matrix_view_vector(tmp_mtr,samples->size2,1);
	
	gsl_matrix* covML = gsl_matrix_alloc(samples->size2,samples->size2);
	gsl_matrix_set_zero(covML);
	for (int i = 0; i< samples->size1; i++){
		gsl_vector_view cur=gsl_matrix_row(samples, i);
		gsl_vector_memcpy(tmp_mtr, &cur.vector);
		gsl_vector_sub(tmp_mtr, meanML);
		
		gsl_blas_dgemm(CblasNoTrans,CblasTrans,
					   1, &tmp_mm.matrix,&tmp_mm.matrix,
					   1, covML);
	}
	gsl_matrix_scale(covML, 1.0/(samples->size1-1));
	gsl_vector_free(tmp_mtr);
	return covML;
}

gsl_matrix* inverse_matrix(gsl_matrix* input){
	size_t n = input->size1;
	int s;
	gsl_matrix	*work = gsl_matrix_alloc(n,n), 
	*winv = gsl_matrix_alloc(n,n);
	gsl_permutation *p = gsl_permutation_alloc(n);
	gsl_matrix_memcpy( work, input);
	gsl_linalg_LU_decomp( work, p, &s );
	gsl_linalg_LU_invert( work, p, winv );
	gsl_matrix_free( work );
	gsl_permutation_free( p );
	return winv;
}

double matrix_determinant(gsl_matrix* input){
	size_t n = input->size1;
	int s;
	gsl_matrix	*work = gsl_matrix_alloc(n,n);
	gsl_permutation *p = gsl_permutation_alloc(n);
	gsl_matrix_memcpy( work, input);
	gsl_linalg_LU_decomp( work, p, &s);
	double output = gsl_linalg_LU_det( work, s );
	gsl_matrix_free( work );
	gsl_permutation_free( p );
	return output;
}

void print_mtrx(gsl_matrix* src){
	char dbl_str[128];
	
	printf("%8s","");
	for (int i = 0; i < src->size2;i++){
		sprintf(dbl_str,"[%d,]",i);
		printf("%10s",dbl_str);
	}
	printf("\n");
	for (int y=0; y< src->size1;y++){
		sprintf(dbl_str,"[%d,]",y);
		printf("%8s",dbl_str);
		for (int x=0; x<src->size2; x++){
			printf(" %.6f,", gsl_matrix_get(src, y, x));
		}
		printf("\n");
	}
}

void print_vec(gsl_vector* src){
	gsl_matrix_view mview = gsl_matrix_view_vector(src, src->size,1);
	print_mtrx(&mview.matrix);
}

void mtrx2tex(gsl_matrix* src, char* cmd_name, FILE* fp){
	fprintf(fp,"\\newcommand{\\%s}{\\[\\begin{pmatrix}\n", cmd_name);
	
	for (int y=0; y< src->size1;y++){
		for (int x=0; x<src->size2; x++){
			fprintf(fp," %.6f %s", gsl_matrix_get(src, y, x),
			x+1==src->size2?"":"&");
		}
		fprintf(fp,"\\\\\n");
	}
	fprintf(fp,"\\end{pmatrix}\\]}\n\n");
}

void vect2tex_trans(gsl_vector* src, char* cmd_name, FILE* fp){
	gsl_matrix_view mv = gsl_matrix_view_vector(src,src->size,1);
	mtrx2tex(&mv.matrix,cmd_name,fp);
}

void vect2tex(gsl_vector* src, char* cmd_name, FILE* fp){
	gsl_matrix_view mv = gsl_matrix_view_vector(src,1,src->size);
	mtrx2tex(&mv.matrix,cmd_name,fp);
}

void norm_vwise(gsl_matrix* trgt){
  gsl_vector_view row;
  double norm;
  for(int i = 0; i< trgt->size1; i++){
    row = gsl_matrix_row(trgt, i);
    norm = 1.0/gsl_blas_dnrm2(&row.vector);
    gsl_vector_scale(&row.vector, norm);
  }
}

double RMS(double* train, double* actual, int length){
  double RMSsquared = 0.0;
  double tmp;
  for(int i = 0; i<length; i++){
    tmp = *(train+i) - *(actual +i);
    RMSsquared += tmp*tmp;
  }
  return sqrt(RMSsquared/length);
}


