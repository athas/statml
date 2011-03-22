#ifndef PPM_H
#define PPM_H

#define MAX_LINE_SIZE 1000

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _OPENM
#define PPM_PARALLEL_FOR #pragma omp parallel for 
#else
#define PPM_PARALLEL_FOR
#endif


typedef struct {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} pnm_pixmap;

typedef struct {
	int width, height;
	int max_color;
	int p;
	void *pixels;
} pnm_img;

enum pnm_image_type {
	ASCII_BITMAP = 1,
	ASCII_GREYMAP = 2,
	ASCII_PIXMAP = 3,
	BINARY_BITMAP = 4,
	BINARY_GREYMAP = 5,
	BINARY_PIXMAP = 6};

typedef struct{
	void* cur_pix;
	void* first_pix;
	double pos_x;
	double pos_y;
	int pixwidth;
} cursor;

void pnm_set_pixel(pnm_img *img, int x, int y, void* pixel);

void* pnm_get_pixel(pnm_img *img, int x, int y);

pnm_img *pnm_create(int width, int height, int p);

pnm_img *pnm_read(const char file_name[]);

void pnm_write(pnm_img *img, const char file_name[]);

void pnm_destroy(pnm_img *img);

/*
File Descriptor 	Type 					Encoding	Note
P1 					Portable bitmap 		ASCII		!Unsuported
P2 					Portable graymap 		ASCII
P3 					Portable pixmap 		ASCII
P4 					Portable bitmap 		Binary		!Unsuported
P5 					Portable graymap 		Binary
P6				 	Portable pixmap 		Binary*/
int pnm_set_imagetype(pnm_img* img, int p);

#ifdef __cplusplus
}
#endif

#endif //PPM_H
