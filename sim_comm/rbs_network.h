#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


#define PI 3.141
#define N_INPUT_LAYER  6        // don't change
#define N_HIDDEN_LAYER 16       // must be devidable by 4
#define N_OUTPUT_LAYER 2        // don't change
#define SLOPE_START 0.5         // start value for sigma (for training)
#define MAX_CHILD 10            // number of childs per generation


typedef struct unit_i{
    double netinput;
    double output;
    double weight; 
} UNIT_I;

typedef struct unit_h{
    double netinput;
    double output;
    double activation;
    double omega;
    double sigma;
} UNIT_H;

typedef struct unit_o{
    double upper_input;
    double lower_input;
    double output;
} UNIT_O;

typedef struct network{
    UNIT_I input_layer [N_INPUT_LAYER];
    UNIT_H hidden_layer[N_HIDDEN_LAYER];
    UNIT_O output_layer[N_OUTPUT_LAYER];
    double fitness;
    double slope;
    int n_input;
    int n_hidden;
    int n_output;
} NETWORK;


typedef struct moving_values{
    short int arr_cntr;
    short int avarage_cntr;
    short int turn_value [256];
    short int l_motor_arr[256]; 
    short int r_motor_arr[256];
    double turn_value_avarage;
    double l_motor_avarage;
    double r_motor_avarage;
    double comb_avarage;
} MOV_VAL;


double my_square(double input);
double gauss();
double calc_output(UNIT_H unit);
void set_start_values(NETWORK *network);
void calc_network(NETWORK *network, int *in_data);
void mutate_network(NETWORK *network);
void generation_step_forward(NETWORK *network, int *child, int max_child);
int get_best_fitness(NETWORK *network, int max_child);
void value_unit(NETWORK *network, MOV_VAL *motor);
short int check_netinput(NETWORK network, short int val);
void set_mov_val(MOV_VAL *motor);
void pre_train(NETWORK *network, int *data_set_in, int *data_set_out, int data_len);
int get_other_best_fitness(double *fitness, int n_child);
double calc_fitness(NETWORK network, double *out_data);