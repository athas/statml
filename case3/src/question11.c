#include "common_case.h"
#include <stdio.h>
#include <math.h>

#define BATCH_SIZE 50
#define NUM_NODES 10


static double sq_dist(vect* v1, vect* v2, int lim){
  double  sum = 0,
  tmp;
  
	for(int i= 0; i<lim; i++){
    tmp = *(v1->data+i) - *(v2->data+i);
		sum+= tmp*tmp;
	}
	return sum;
}

typedef struct {
  mtrx* hidden;
  vect* output;
  double lr_hin;
  double lr_hout;
} ff_nn_t;

ff_nn_t* init_weights(
                      int num_input, 
                      int num_hidden_nodes, 
                      double weight_fac,
                      double lr_hin,
                      double lr_hout){
  mtrx* h_in_w=gsl_matrix_alloc(num_hidden_nodes, num_input+1);
  vect* h_out_w=gsl_vector_alloc(num_hidden_nodes);

  gsl_rng * r = gsl_rng_alloc(gsl_rng_taus);

  for(int i = 0; i < h_in_w->size1; i++){
    gsl_matrix_set(h_in_w, i, 0, 1.0); //bias
    gsl_vector_set(h_out_w, i, gsl_rng_uniform(r));
    for(int j = 1; j < h_in_w->size2; j++){
      gsl_matrix_set(h_in_w, i, j, gsl_rng_uniform(r));
    }
  }
  gsl_rng_free(r);
  
  gsl_matrix_scale(h_in_w, weight_fac);
  gsl_vector_scale(h_out_w, weight_fac);
  ff_nn_t* out = (ff_nn_t*)malloc(sizeof(ff_nn_t));
  out->hidden = h_in_w;
  out->output = h_out_w;
  out->lr_hin = lr_hin;
  out->lr_hout = lr_hout;
  return out;
}


/* programming very specifcally to the assignment here, should be generalized
 for any output size*/
double ffnn_predict(
                    vect* inp, 
                    ff_nn_t* weights, 
                    vect* hidden_output, 
                    double(*trfn)(double)){
  
  double tmp;
  gsl_vector_view wview;
  for (int i = 0; i < inp->size; i++){
    wview = gsl_matrix_row(weights->hidden, i);
    gsl_blas_ddot(&wview.vector, inp, &tmp); //multiply and sum
    tmp = trfn(tmp); //fire function
    gsl_vector_set(hidden_output, i, tmp);
  }
  gsl_blas_ddot(hidden_output, weights->output, &tmp); //ddot: mul and sum
  return tmp;
}


static void backpropagate(ff_nn_t* w, vect* hidden_res, double diff, vect* input){
  mtrx* HI_weight = w->hidden;
  vect* HO_weight = w->output;

  for(int i = 0; i < w->output->size; i++){
    double hid_val_i = gsl_vector_get(hidden_res, i);
    
    double ho_i_weight = gsl_vector_get(HO_weight, i);
    ho_i_weight -=(w->lr_hout*diff*hid_val_i);
    gsl_vector_set(HO_weight,i, ho_i_weight);
    
    double hi_weightChange = 1-(hid_val_i* hid_val_i);//is this right?
    hi_weightChange *= ho_i_weight * diff * w->lr_hin;
    
//    gsl_vector_set(w->output, i, );
    for(int j = 1; j< input->size; j++){ //note that bias is skipped
      double hi_ij_weight = gsl_matrix_get(HI_weight, i, j);
      hi_ij_weight -= (hi_weightChange*gsl_vector_get(input, j));
      gsl_matrix_set(HI_weight, i, j, hi_ij_weight);
    }
  }
}

double batch_train(ff_nn_t* weights, mtrx* data, double(*trans_fn)(double)){
  mtrx* intermediaries=gsl_matrix_alloc(BATCH_SIZE,NUM_NODES+1);
  //columns above: hidden_output && result
  
  gsl_vector_view h_res;
  gsl_vector_view inp;
  for(int i =0;i<BATCH_SIZE; i++){
    h_res = gsl_matrix_subrow(intermediaries,i,0,NUM_NODES); //bad magic const
    inp = gsl_matrix_subrow(data, i, 0, 2);
    gsl_matrix_set(intermediaries,i,weights->output->size,
                   ffnn_predict(&inp.vector, weights, &h_res.vector,trans_fn));
  }
  
  double sq_cum_err = 0.0;
  for(int i =0;i<BATCH_SIZE; i++){
    inp = gsl_matrix_subrow(data, i, 0, 2);
    h_res = gsl_matrix_subrow(intermediaries,i,0,weights->output->size);
    double tmp_res = gsl_matrix_get(intermediaries,i,NUM_NODES);//bad magic const
    double local_error = tmp_res - gsl_matrix_get(data, i, 2);
    sq_cum_err+= local_error*local_error;
    backpropagate(weights, &h_res.vector, local_error, &inp.vector);
//    printf("iteration: %d\terror: %f\n", i, local_error);
  }
 // free(move_stack_pointer_slightly);
  gsl_matrix_free(intermediaries);
  return sqrt(sq_cum_err);
}

double trans_fn(double inp){
  double tmp = inp/(1+ (inp<0?-inp:inp));
  return tmp < 0?-1.0:1.0;
}

int main(int argv, char** argc){
  mtrx* data = gsl_matrix_alloc(BATCH_SIZE, 3);
  gsl_vector_view bias_col = gsl_matrix_column(data, 0);
  gsl_matrix_view d_file_data = gsl_matrix_submatrix(data, 0, 1, BATCH_SIZE,2);
  
  file2mtrx("../Data/sincTrain50.dt", &d_file_data.matrix);
  gsl_vector_set_all(&bias_col.vector, 1.0);

  ff_nn_t* weights = init_weights(1, NUM_NODES, 1.0, 0.07, 0.7);
  
  for(int i = 0; i< 50;i++)
    printf("iteration: %d\terror: %f\n", i, batch_train(weights, data, trans_fn));
  

  print_mtrx(weights->hidden);
  print_mtrx(data);
  print_vec(weights->output);
  mtrx2file(data, "fig_out/normsinctrain.dt");
}
