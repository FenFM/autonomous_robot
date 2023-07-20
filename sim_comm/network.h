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
#define N_INPUT_LAYER  6
#define N_HIDDEN_LAYER_1 16                 // must be devidable by 4
#define N_HIDDEN_LAYER_2 N_HIDDEN_LAYER_1
#define N_OUTPUT_LAYER 2
#define N_OF_DATA_SETS 2
#define SLOPE_START 2                       // start value for sigma (for training)
#define MAX_CHILD 10                        // number of childs per generation


typedef struct unit{
    double netinput;
    double output;
    double activation;
    double weight;
} UNIT;


typedef struct network{
    UNIT input_layer   [N_INPUT_LAYER] ;
    UNIT hidden_layer_1[N_HIDDEN_LAYER_1];
    UNIT hidden_layer_2[N_HIDDEN_LAYER_2];
    UNIT output_layer  [N_OUTPUT_LAYER];
    double fitness;   
    double slope; 
    int n_input;
    int n_hidden_1;
    int n_hidden_2;
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
    short int comb_avarage;
} MOV_VAL;


double my_square(double input);
double gauss();
double calc_output(UNIT unit);
void calc_network(NETWORK *network, double *in_data);
void mutate_network(NETWORK *network);
void generation_step_forward(NETWORK *network, int *child, int max_child);
void set_start_values(NETWORK *network);
double calc_fitness(NETWORK network, double *out_data);
int get_best_fitness(NETWORK *network, int max_child);
void value_unit(NETWORK *network, MOV_VAL *motor);
short int check_netinput(NETWORK network, short int val);
void set_mov_val(MOV_VAL *motor);