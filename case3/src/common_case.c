#include "common_case.h"
#include <errno.h>
#include <string.h>
#include <unistd.h> 


#define MAX_LINE_SIZE 4096

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
static inline void line2mtrx_row(mtrx* trgt, int rowno, char* buf){
  for (int j= 0; j< trgt->size2; j++){
    gsl_matrix_set(trgt, rowno, j,  strtod(buf, &buf));
  }
}


int file2mtrx(const char file_name[], mtrx* trgt){
  int max_line_size = (int)(32 * trgt->size1);
  char* buffer = (char*) malloc(sizeof(char)*max_line_size);
	FILE* fp;
  
	// open file
	fp = fopen(file_name,"r"); 
	if(fp == NULL) {
		path_error(file_name);
    fclose(fp);
		return -1;
	}
  
  /*	if(buffer[0] != '#') {
   printf("Bad header format\n");
   return NULL;
   }
   */
  char* status;
	for (int i = 0; i< trgt->size1; i++){
		status = fgets(buffer, max_line_size, fp);
		if(status == NULL) {
			printf("Unexpted end of matrix file.\n");
      fclose(fp);
			return -1;		
		}
    line2mtrx_row(trgt, i, buffer);
	}
	fclose(fp);
	return 0;
}

void mtrx2file (mtrx* output, char* fname){
	FILE* fp;
	fp = fopen(fname, "w");
	
  #pragma omp parallel 
	for (int i = 0; i< output->size1; i++){
    for (int j = 0; j< output->size2; j++){
      fprintf(fp, "%lf ", gsl_matrix_get(output, i,j));
    }
    fprintf(fp, "\n");
  }
fclose(fp);
}
