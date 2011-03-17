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

static inline int max_offset(int k, keighbor_t* cknn){
	int max=cknn->sq_dist;
	for (int i = 0;i<k;i++)
		if ((cknn+i)->sq_dist > (cknn + max)->sq_dist)
			max = i;
	return max;
}

static inline int knn_in_nearest(int subj, int K, keighbor_t* cknn){
  for(int i = 0; i < K; i++)
    if(subj == (cknn+i)->offset)
      return 1;
  return 0;
}

static void test_nearest(int k, keighbor_t* cknn,vect* subj, mtrx* others, vect* wrk, int cur_max, dist_fun_t d_fun){
  int max = max_offset(k, cknn);
  if(max != cur_max)
    printf("Differs on max inside nearest! old: %d new: max%d\n", (cknn+max)->offset, (cknn +cur_max)->offset);
  
  for (int i = 0; i < others->size1; i++) {
		gsl_vector_view row=gsl_vector_view_array(others->data+i*others->tda,subj->size);
		double tmp = d_fun(subj, &row.vector,k);
		if (tmp	<= (cknn+max)->sq_dist && !knn_in_nearest(i, k, cknn)){
      printf("found lower NN at %d\n", i);
		}
	}
}


void k_nearest(int k, vect* subj, mtrx* others,keighbor_t* nearest, dist_fun_t d_fun){
	int n = others->size1;
  
	double tmp;	
	for (int i =0; i<k; i++){
    gsl_vector_view row=gsl_vector_view_array(others->data+i*others->tda,subj->size);
		(nearest+i)->sq_dist = d_fun(subj, &row.vector, 2);
		(nearest+i)->offset = i;
    (nearest+i)->value = gsl_vector_get(&row.vector,others->size2-1);
	}
  int cur_max = max_offset(k, nearest);

	for (int i = k; i < n; i++) {
		gsl_vector_view row=gsl_vector_view_array(others->data+i*others->tda,others->size2);
		tmp = d_fun(subj, &row.vector, 2);
		if (tmp	<= (nearest+cur_max)->sq_dist){
      (nearest+cur_max)->sq_dist = tmp;
      (nearest+cur_max)->offset = i;
      (nearest+cur_max)->value = gsl_vector_get(&row.vector,others->size2-1);
			cur_max = max_offset(k, nearest);
		}
	}
#ifdef SANITYCHECK
  test_nearest(k, nearest,subj, others, wrk,cur_max, d_fun);
#endif  
}

void print_knn(int K, keighbor_t* cknn){
  printf("K = %d:\n",K);
	for (int i =0; i<K; i++)
		printf("\tindex:%d, dist:%f, val:%d\n",
           (cknn+i)->offset,(cknn+i)->sq_dist,(cknn+i)->value);
}

int knn_knoll_train_decide(keighbor_t* cknn, vect* subj, int K){
  int result = 0;
  for(int i = 0; i < K; i++)
    result += (cknn+i)->value;
  result = result > 0?1:-1;
  return result;
}

double knn_knoll_hitrate(mtrx* train, mtrx* test, int K, dist_fun_t d_fun){
  int n_miss = 0;
  keighbor_t* cknn = (keighbor_t*)malloc(sizeof(keighbor_t)*K);
  gsl_vector_view subj;
  
  for(int i = 0; i<test->size1; i++){
    subj = gsl_matrix_row(test,i);
    k_nearest(K, &subj.vector, train,cknn, d_fun);    
    int trained = knn_knoll_train_decide(cknn, &subj.vector, K);
    int actual = (int)*(subj.vector.data+test->size2-1);
    n_miss += trained == actual?0:1;
  }
  free(cknn);
  return (test->size1 - (double)n_miss)/test->size1;
}

void gplot_knn2plot(mtrx* coords, char* fname){
	FILE* fp;
	fp = fopen(fname, "w");
	
	for (int i = 0; i< coords->size1; i++){
    fprintf(fp, "%f %f\n", 
            gsl_matrix_get(coords, i,0),
            gsl_matrix_get(coords, i,1));
  }
fclose(fp);
}