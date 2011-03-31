#include "common_case.h"
#include <stdio.h>
#include <math.h>



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
  int num_nodes;
  int input_size;
  int output_size;
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
  out->num_nodes = num_hidden_nodes;
  out->input_size = num_input;
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
  for (int i = 0; i < weights->num_nodes; i++){
    wview = gsl_matrix_row(weights->hidden, i);
    gsl_blas_ddot(&wview.vector, inp, &tmp); //multiply and sum
    tmp = trfn(tmp); //fire function
    gsl_vector_set(hidden_output, i, tmp);
  }
  gsl_blas_ddot(hidden_output, weights->output, &tmp); //ddot: mul and sum
  return tmp/weights->num_nodes;
}


static void backpropagate(ff_nn_t* w, vect* hidden_res, double diff, vect* input, mtrx* gradient){
  mtrx* HI_weight = w->hidden;
  vect* HO_weight = w->output;

  for(int i = 0; i < w->num_nodes; i++){
    double hid_val_i = gsl_vector_get(hidden_res, i);
    
    double ho_i_weight = gsl_vector_get(HO_weight, i);
    double ho_delta = (diff>0?diff:-diff)*hid_val_i;
    gsl_vector_set(HO_weight,i, ho_i_weight-(ho_delta*w->lr_hout));
    
    double hi_delta = 1-(hid_val_i* hid_val_i)*ho_delta;
    hi_delta *= gsl_vector_get(input, 1); 
    
    gsl_matrix_set(gradient, i,2, ho_delta*gsl_matrix_get(HI_weight, i, 1));
    gsl_matrix_set(gradient, i,0, hi_delta);
    for(int j = 0; j< input->size; j++){ 
      double hi_ij_weight = gsl_matrix_get(HI_weight, i, j);
      hi_ij_weight -= (hi_delta*gsl_vector_get(input, j))*w->lr_hin;
      gsl_matrix_set(HI_weight, i, j, hi_ij_weight);
      gsl_matrix_set(gradient, i, j+1, hi_delta*hi_ij_weight);
    }
  }
}

double batch_train(ff_nn_t* weights, mtrx* data, double(*trans_fn)(double), mtrx* grad){
  mtrx* intermed=gsl_matrix_alloc(data->size1,weights->num_nodes+1);
  //columns above: hidden_output && result
  gsl_vector_view h_res;
  gsl_vector_view inp;
  for(int i =0;i<data->size1; i++){
    h_res = gsl_matrix_subrow(intermed,i,0,weights->num_nodes);
    inp = gsl_matrix_subrow(data, i, 0, 2);
    gsl_matrix_set(intermed,i,weights->output->size,
                   ffnn_predict(&inp.vector, weights, &h_res.vector,trans_fn));
  }
  
  double mean_err = 0.0;
  for(int i =0;i<data->size1; i++){
    inp = gsl_matrix_subrow(data, i, 0, 2);
    h_res = gsl_matrix_subrow(intermed,i,0,weights->num_nodes);
    double tmp_res = gsl_matrix_get(intermed,i,weights->num_nodes);
    double local_error = tmp_res - gsl_matrix_get(data, i, 2); //sub actual
    mean_err+= local_error;
    backpropagate(weights, &h_res.vector, local_error, &inp.vector, grad);
  }
  gsl_matrix_free(intermed);
  mean_err/=weights->num_nodes;
  return mean_err*mean_err;
}

double trans_fn(double inp){
  return inp/(1+ (inp<0?-inp:inp));
}

int main(int argv, char** argc){
  mtrx* data = gsl_matrix_alloc(50, 3);
  gsl_vector_view bias_col = gsl_matrix_column(data, 0);
  gsl_matrix_view d_file_data = gsl_matrix_submatrix(data, 0, 1, 50,2);
  
  file2mtrx("../Data/sincTrain50.dt", &d_file_data.matrix);
  
  double LR = 0.00000001;
  int batch_size = 50;
  double delta_threshold = 0.0001;
  
  for (int n_hn = 2; n_hn <21;n_hn+=18){
    int num_its = 0;
    double cur_error = -1.0;
    double prev_error;
    ff_nn_t* weights = init_weights(1, n_hn, 1, LR, LR*3);
    gsl_vector_set_all(&bias_col.vector, 1.0);
    mtrx* gradient = gsl_matrix_alloc(n_hn,2+1);
    do{
      prev_error = cur_error;
      cur_error = batch_train(weights, data, trans_fn, gradient);
      num_its++;
//      printf("%f\n", cur_error);
    } while(cur_error>delta_threshold);

    printf("hnodes:%d batch_size:%d LR:%f converged@%f in %d its\n", 
           n_hn, batch_size, LR, cur_error, num_its);
    print_mtrx(gradient);
    gsl_matrix_free(gradient);
  }
}
