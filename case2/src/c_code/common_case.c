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

gsl_matrix* read_knoll(const char file_name[]){
	FILE* fp;
	char buffer[MAX_LINE_SIZE];

	// open file
	fp = fopen(file_name,"r"); 
	if(fp == NULL) {
		path_error(file_name);
		return NULL;
	}

	if(read_line(buffer,MAX_LINE_SIZE,fp)){
        printf("unexpected first line of knoll file.\n");
        return NULL;
    }
	if(buffer[0] != '#') {
		printf("Bad header format\n");
		return NULL;
	}
	int n_lines, size1, size2;
	sscanf(buffer+1,"%d %d %d", &n_lines, &size1, &size2);
	
	mtrx* out = gsl_matrix_alloc(n_lines, 3);
	
	double x1, x2, x3;
	int status;
	for (int i = 0; i< n_lines; i++){
		status = fscanf(fp,"%lf %lf %lf\n", &x1, &x2, &x3);
		if(status == EOF) {
			printf("Bad file format\n");
			return NULL;		
		}
		gsl_matrix_set(out, i, 0, x1);
		gsl_matrix_set(out, i, 1, x2);
		gsl_matrix_set(out, i, 2, x3);
	}
	
	fclose(fp);
	return out;
}


static inline double sq_dist(vect* v1, vect* v2, vect* wrk){
    double  sum = 0,
            tmp;
    
	for(int i= 0; i<v1->size; i++){
        tmp = *(v1->data+i) - *(v2->data+i);
		sum+= tmp*tmp;
	}
	return sum;
}

static inline int max_offset(int k, keighbor_t* cknn){
	int max=cknn->sq_dist;
	for (int i = 0;i<k;i++)
		if ((cknn+i)->sq_dist > (cknn + max)->sq_dist)
			max = i;
	return max;
}

static void test_nearest(int k, keighbor_t* cknn,vect* subj, mtrx* others, vect* wrk, int cur_max){
    int max = max_offset(k, cknn);
    if(max != cur_max)
        printf("Differs on max inside nearest! old: %d new: max%\n", (cknn+max)->offset, (cknn +cur_max)->offset);
        
    for (int i = 0; i < others->size1; i++) {
		gsl_vector_view row=gsl_vector_view_array(others->data+i*others->tda,subj->size);
		double tmp = sq_dist(subj, &row.vector, wrk);
		if (tmp	<= (cknn+max)->sq_dist){
            printf("found lower NN at %d\n", i);
		}
	}
}


keighbor_t* k_nearest(int k, vect* subj, mtrx* others){
	int n = others->size1,
		cur_max = 0;
	double tmp;
	vect* wrk = gsl_vector_alloc(subj->size);
	keighbor_t* nearest = malloc(sizeof(keighbor_t)*k);
	
	for (int i =0; i<k; i++){
        gsl_vector_view row=gsl_vector_view_array(others->data+i*others->tda,subj->size);
		(nearest+i)->sq_dist = sq_dist(subj, &row.vector, wrk);
		(nearest+i)->offset = i;
	}

	for (int i = k; i < n; i++) {
		gsl_vector_view row=gsl_vector_view_array(others->data+i*others->tda,subj->size);
		tmp = sq_dist(subj, &row.vector, wrk);
		if (tmp	<= (nearest+cur_max)->sq_dist){
            (nearest+cur_max)->sq_dist = tmp;
            (nearest+cur_max)->offset = i;
			cur_max = max_offset(k, nearest);
		}
	}
    test_nearest(k, nearest,subj, others, wrk,cur_max);


	return nearest;
}

