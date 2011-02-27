#include "common_case.h"
#include <math.h>

pnm_img* pnm_subimage(pnm_img* src, int* extent){
	int width = extent[1] - extent[0];
	int height = extent[3] - extent[2];
	pnm_img* out_img = pnm_create(width, height, BINARY_PIXMAP);
	
	for(int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			pnm_pixmap* p = pnm_get_pixel(src,x+*extent,y+*(extent+2));
			pnm_set_pixel(out_img,x,y,p);
		}
	}
	return out_img;
}

gsl_matrix* img2train_set(pnm_img* img){
	gsl_matrix* train_set = gsl_matrix_alloc(img->width*img->height, 3);
	
	pnm_pixmap* p = (pnm_pixmap*)img->pixels;
	for(int y = 0; y < img->height; y++, p++){
		for (int x = 0; x < img->width; x++){
			gsl_vector_view cr = gsl_matrix_row(train_set, y*img->width+x);
			gsl_vector_set(&cr.vector, 0, p->R);
			gsl_vector_set(&cr.vector, 1, p->G);
			gsl_vector_set(&cr.vector, 2, p->B);
		}
	}
	return train_set;
}

inline void pix2vec(pnm_pixmap* pix, gsl_vector* x){
	gsl_vector_set(x, 0, pix->R);
	gsl_vector_set(x, 1, pix->G);
	gsl_vector_set(x, 2, pix->B);		
}

double pdf(gsl_vector* x_m_s, gsl_matrix* cov_inv){
	gsl_matrix_view mv = gsl_matrix_view_vector(x_m_s, 1, x_m_s->size);
	gsl_matrix* tmp = gsl_matrix_alloc(1, x_m_s->size);
	gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,
				   1,&mv.matrix, cov_inv,
				   0 ,tmp);
	gsl_vector_view rv = gsl_matrix_row(tmp,  0);
	double almost_there;
	gsl_blas_ddot(&rv.vector, x_m_s, &almost_there);
	
	gsl_matrix_free(tmp);
	return exp(-0.5*almost_there);
}
