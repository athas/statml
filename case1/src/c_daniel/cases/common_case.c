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

inline void pix2vec(pnm_pixmap* pix, gsl_vector* x){
	gsl_vector_set(x, 0, pix->R);
	gsl_vector_set(x, 1, pix->G);
	gsl_vector_set(x, 2, pix->B);		
}


gsl_matrix* img2train_set(pnm_img* img){
	gsl_matrix* train_set = gsl_matrix_alloc(img->width*img->height, 3);
	pnm_pixmap* p = (pnm_pixmap*)img->pixels;
	for(int y = 0; y < img->height; y++){
		for (int x = 0; x < img->width; x++,p++){
			gsl_vector_view cr = gsl_matrix_row(train_set, y*img->width+x);
			pix2vec(p,&cr.vector);
		}
	}
	return train_set;
}


double pdf(gsl_vector* x_m_s, gsl_matrix* cov_inv,gsl_matrix* tmp){
	gsl_matrix_view mv = gsl_matrix_view_vector(x_m_s, 1, x_m_s->size);
	
	//  allocating and freeing the tmp took 97% of calc time. 
	//  Let that be a lesson!
	//	gsl_matrix* tmp = gsl_matrix_alloc(1, x_m_s->size);
	gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,
				   1,&mv.matrix, cov_inv,
				   0 ,tmp);
	gsl_vector_view rv = gsl_matrix_row(tmp,  0);
	double almost_there;
	gsl_blas_ddot(&rv.vector, x_m_s, &almost_there);
	
	//	gsl_matrix_free(tmp);
	return exp(-0.5*almost_there);
}

static inline double dmax(double a, double b){
	return a > b ? a : b;}


double pdf_map(pnm_img* img, vect* sigma, mtrx* cov, double** map){
	double one_over = 1.0/pow(2*M_PI,3/2) * sqrt(matrix_determinant(cov));
	double max = 0;
	
	mtrx* cov_inv = inverse_matrix(cov);
	
	vect* x = gsl_vector_alloc(3);
		// working mtrx for pdf()
	gsl_matrix* tmp = gsl_matrix_alloc(1, x->size);
	
	pnm_pixmap* pix = (pnm_pixmap*)img->pixels;
	for (int i =0; i<img->width*img->height; i++,pix++) {
		pix2vec(pix, x);
		gsl_vector_sub(x,sigma);
		*(*map+i) = pdf(x, cov_inv, tmp)*one_over;
		max = dmax(max,*(*map+i));
	} 
	gsl_matrix_free(cov_inv);
	gsl_vector_free(x);
	
	return max;
}

pnm_img* img_pdf(pnm_img* img, vect* sigma, mtrx* cov){
	
	pnm_img* out = pnm_create(img->width, img->height, BINARY_GREYMAP);
	double* map = (double*)malloc(img->width*img->height*sizeof(double));
	
	double normalized = 1.0/pdf_map(img, sigma,cov,&map);
	
	for (int i =0; i<img->width*img->height; i++) {
		*((unsigned char*)out->pixels+i) = 0xff*normalized* (*(map+i));
	}
	
	free (map);
	return out;
}

/*mtrx* refined_img2train(pnm_img* img, vect* sigma, mtrx* cov){
	double* map = (double*)malloc(img->width*img->height*sizeof(double));
	int pos=0;
	pnm_pixmap* pix = (pnm_pixmap*)img->pixels;
	double max_norm = pdf_map(img, sigma, cov, &map)*0.1; //30%
	for (int i =0; i<img->width*img->height; i++){
		if (*(map+i) > max_norm){
			*(pix+pos++) = *(pix+i);
		}
	}
	img->width= pos;
	img->height = 1;
	free (map);
	
	return img2train_set(img);
}
*/


void gplot_img2splot(pnm_img* img, double Z, char* fname){
	FILE* fp;
	fp = fopen(fname, "w");
	pnm_pixmap* pix = (pnm_pixmap*)img->pixels;
	
	for (int iy=1; iy<=img->height; iy++){
		for (int ix=0; ix<img->width; ix++){
			int pos = ix+(img->height-iy)*img->width;
			fprintf(fp, "%d %d %f %d %d %d\n", 
				ix,iy, Z,
				(pix+pos)->R,(pix+pos)->G,(pix+pos)->B);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void gplot_pdf2splot(vect* mean, mtrx* cov, pnm_img* img, int skip, char* fname){
	
	FILE* fp;
	fp = fopen(fname, "w");

	double Z = 1.0/( (2*M_PI) * sqrt(matrix_determinant(cov)));
	mtrx* cov_inv = inverse_matrix(cov);
	vect* x = gsl_vector_alloc(2);
		// working mtrx for pdf()
	gsl_matrix* tmp = gsl_matrix_alloc(1, x->size);
	
	for (int iy=1; iy<=img->height; iy+=skip){
		for (int ix=0; ix<img->width; ix+=skip){
			gsl_vector_set(x, 0, ix);
			gsl_vector_set(x, 1, img->height-iy);
			gsl_vector_sub(x,mean);
			fprintf(fp, "%d %d %.16f \n", 
					ix,iy,Z * pdf(x, cov_inv, tmp));
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}



vect* weighted_avg_pos(pnm_img* img, double* map){
	
	double 	X = 0.0,
	Y = 0.0;
	double 	sum = 0.0;
	
	vect* output = gsl_vector_alloc(2);
	for (int iy =0; iy<img->height; iy++){
		for (int ix=0; ix<img->width; ix++){
			int pos = ix+iy*img->width;
			X+= ix*(*(map+pos));
			Y+= iy*(*(map+pos));
			sum+=*(map+pos);
		}
	}
	X /= sum;
	Y /= sum;
	
	gsl_vector_set(output, 0, X);
	gsl_vector_set(output, 1, Y);
	
	return output;
}

mtrx* weighted_2dcov(double* weights, vect* s_mean, pnm_img* img){
	gsl_matrix* tmp_mm = gsl_matrix_alloc(2, 1);
	gsl_matrix* covML = gsl_matrix_alloc(2,2);
	gsl_matrix_set_zero(covML);
	double sum = 0.0;
	for (int iy =0; iy<img->height; iy++){
		for (int ix=0; ix<img->width; ix++){
			int pos = ix+iy*img->width;	
			sum+=weights[pos];
			gsl_matrix_set(tmp_mm, 0, 0, ix - *s_mean->data);
			gsl_matrix_set(tmp_mm, 1, 0, iy -*s_mean->data+1);
			gsl_blas_dgemm(CblasNoTrans,CblasTrans,
						   weights[pos], tmp_mm, tmp_mm,
						   1, covML);
		}
	}
	gsl_matrix_scale(covML, 1.0/sum);
	return covML;
}
