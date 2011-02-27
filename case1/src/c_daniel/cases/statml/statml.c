#include "statml.h"

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
	int n = input->size1;
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
	int n = input->size1;
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

void print_mtrx(gsl_matrix* src, char* dbl_fmt){
	char dbl_str[128];
	for (int y=0; y< src->size1;y++){
		for (int x=0; x<src->size2; x++){
			snprintf(dbl_str, 128, dbl_fmt, gsl_matrix_get(src, y, x));
			printf("%s",dbl_str);
		}
		printf("\n");
	}
}

void print_vec(gsl_vector* src, char* dbl_fmt){
	gsl_matrix_view mview = gsl_matrix_view_vector(src, 1, src->size);
	print_mtrx(&mview.matrix,dbl_fmt);
}

