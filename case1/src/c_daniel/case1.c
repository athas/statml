#include "ppm.h"
#include <stdio.h>

/* extent layout:
 * {xmin,xmax,ymin,ymax}*/
#define TRAIN_AREA 	{150-1,330-1,264-1,328-1};

void train(ppm_img* img, int* extent){
	int width = extent[1] - extent[0];
	int height = extent[3] - extent[2];
	
	printf ("w:%d, h:%d\n", width, height);
	
	ppm_img* out_img = ppm_create(width, height, BINARY_PIXMAP);

	for (int x = *extent; x <= *(extent+1); x++){
		for(int y = *(extent+2); y <= *(extent+3); y++){
			ppm_pixmap* p = ppm_get_pixel(img,x,y);
			ppm_set_pixel(out_img,x-*extent,y-*(extent+2),p);
		}
	}
	/* write image to file, then free image memory*/
	ppm_write(out_img, "img_out.pbm");
	ppm_release(out_img);
}

int main(int argv, char** argc){	
	/*read image from file*/
	ppm_img* input_img = ppm_read("kande1.ppm");
	
	/*convert image type from grayscale to RGB*/
	ppm_set_imagetype(input_img, BINARY_PIXMAP);
	
	int train_area[] = TRAIN_AREA;
	train(input_img,train_area);
}