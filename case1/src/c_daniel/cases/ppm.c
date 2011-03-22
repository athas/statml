#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "ppm.h"
#include <unistd.h> 

inline static int num_channels(int p){
	switch (p){
		case 1:
		case 4:
		printf("Bitmaps not supported!\n");
		return -1;
		case 2:
		case 5:
		return 1;
		case 3:
		case 6:
		return 3;
		default:
		printf("Unrecognized PPM format:p%d\n",p);
		return -1;
	}
}

void* pnm_get_pixel(pnm_img* img, int x, int y){
    return ((char*)img->pixels)+(y*img->width+x)*num_channels(img->p);
}

void pnm_set_pixel(pnm_img *img, int x, int y, void* pixel) {
	void* p =pnm_get_pixel(img, x,y);
	if (num_channels(img->p) == 1)
		*((char*)p) = *((char*)pixel);
	else
		*((pnm_pixmap*)p) = *((pnm_pixmap*)pixel);
}

//cache align coordinates in squares instead of lines
inline static int intermediate_sq_offset(int x, int y, int width){
	return ((y*width)+x)<<6;
}


pnm_img *pnm_create(int width, int height, int p) {
	pnm_img *img = (pnm_img*) malloc(sizeof(pnm_img));
	img->width = width;
	img->height = height;
	img->max_color = 255;
	img->p = p;
	img->pixels= calloc(height*width* num_channels(p),sizeof(char));
	return img;
}

static inline int read_line(char* buffer, const int max_size, FILE* fp){
	if(!fgets(buffer,MAX_LINE_SIZE,fp)){
		printf("Bad file format!\n");
		return -1;
	}
	else
		return 0;
} 

static void path_error(const char* file_name){
	char path[512];
	getcwd(path, 512);
	
	printf("\"%s%s\" does not exist\n", path, file_name);
}

pnm_img *pnm_read(const char file_name[]){
	FILE* fp;
	int status;
	int p;
	char buffer[MAX_LINE_SIZE];

	// open file
	fp = fopen(file_name,"r"); 
	if(fp == NULL) {
		path_error(file_name);
		return NULL;
	}
	
	// get PPM type	
	read_line(buffer,MAX_LINE_SIZE,fp);
	if(buffer[0] != 'P') {
		printf("Bad header format\n");
		return NULL;
	}
	p = atoi(buffer+1);
	// read past any comments
	do {
		read_line(buffer,MAX_LINE_SIZE,fp);
	} while(buffer[0] == '#');

	// get dimensions 
	char* current_position;
	int width = strtol(buffer,&current_position,10);
	if (*current_position == '\n'){
		read_line(buffer,MAX_LINE_SIZE,fp);
		current_position = buffer;
	}
	int height = strtol(current_position,&current_position,10);

	pnm_img* img = pnm_create(width, height, p);

	if (*current_position == '\n'){
		read_line(buffer,MAX_LINE_SIZE,fp);
		current_position = buffer;
	}
	img->max_color = strtol(current_position,&current_position,10);

	// extract pixel data
	if(p == 5 || p == 6){
		fread(img->pixels, sizeof(char),height*width*num_channels(p),fp);
	}
	else{
		int R, G, B;
		if (num_channels(p) == 3){
			pnm_pixmap* pm = (pnm_pixmap*)img->pixels;
			for(int i = 0; i< height*width; i++){
				status = fscanf(fp," %i %i %i", &R, &G, &B);
				if(status == EOF) {
					printf("Bad file format\n");
					return NULL;		
				}
				pm[i].R = (unsigned char)R;
				pm[i].G = (unsigned char)G;
				pm[i].B = (unsigned char)B;
			}
		}
		else{
			int grey;
			for(int i = 0; i< height*width; i++){
				status = fscanf(fp," %i", &grey);
				if(status == EOF) {
					printf("Bad file format\n");
					return NULL;		
				}
				*((unsigned char*)img->pixels+i) = (unsigned char)grey;
			}
		}
	}
	fclose(fp); 
	return img;
}

void pnm_write(pnm_img *img, const char file_name[]) {
	int linewidth = 0;
	FILE* fp;
	fp = fopen(file_name, "w");

	if(img->p >1 && img->p <4){
		fprintf(fp, "P%d\n%d %d\n%d\n", 
			img->p,img->width, img->height, img->max_color);
		if(num_channels(img->p) == 3){
			pnm_pixmap* pm = (pnm_pixmap*)img->pixels;
			for(int i = 0; i<img->height*img->width; i++) {
				linewidth += 3*3+3+1;
				fprintf(fp, "%i %i %i  %s", 
					(int)(pm[i].R),
					(int)(pm[i].G),
					(int)(pm[i].B),
					linewidth >= 72?"\n":"");
					if (linewidth >= 72)
						linewidth = 0;
			}
		}
		else {
			unsigned char* p = (unsigned char*)img->pixels;
			for(int i = 0; i<img->height*img->width; i++) {
				linewidth += 3+2;
				fprintf(fp, "%i  %s", 
					(int)p[i], linewidth >= 72?"\n":"");
					if (linewidth >= 72)
					linewidth = 0;
			}
		}
	}
	else if (img->p>4){
		fprintf(fp, "P%d\n%d %d\n%d\n", 
				img->p,img->width, img->height, img->max_color);
		fwrite(img->pixels, sizeof(unsigned char)*num_channels(img->p),
				img->height * img->width, fp);
	}
	fclose(fp);
}

static void pnm_to_grayscale(pnm_img *img){
	if (num_channels(img->p)==1){
		printf("pnm_to_grayscale: already a greyscale image!\n");
		return;
	}
	pnm_pixmap* p = (pnm_pixmap*)img->pixels;
	unsigned char* g = (unsigned char*)img->pixels;
	int n_pixels = img->width*img->height;
	for(int i=0; i<n_pixels;i++,p++,g++)
		*g = (p->R + p->G + p->B)/3;
	img->pixels = realloc(img->pixels, sizeof(char)*n_pixels);
}

static void pnm_to_rgb(pnm_img *img) {
	if (num_channels(img->p)==3){
		printf("pnm_to_rgb: already an rgb image!\n");
		return;
	}
	int n_pixels = img->height*img->width;
	pnm_pixmap* pm = (pnm_pixmap*)malloc(sizeof(pnm_pixmap)*n_pixels);
	pnm_pixmap pv = {0,0,0};
	for(int i=0; i<n_pixels; i++){
		pv.R = pv.G = pv.B = *((unsigned char*)img->pixels+i);
		*(pm+i) = pv;
	}
	free(img->pixels);
	img->pixels = pm;
}

void pnm_destroy(pnm_img *img) {
	free(img->pixels);
	free(img);
}

int pnm_set_imagetype(pnm_img* img, int p){
	int oc, nc;
	nc = num_channels(p);
	if (nc == -1)
		return -1;
	oc = num_channels(img->p);

	if (img->p == p)
		return 0;
	
	if (nc == oc){ /* same colour depth, fileformat difference */
		img->p = p;
		return 0;
	}
	else{
		if(oc == 3){
			pnm_to_grayscale(img);
		}
		else{
			pnm_to_rgb(img);
		}
	}
	img->p = p;
	return 0;
}
