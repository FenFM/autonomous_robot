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
#define SLOPE_START 0.1           // start value for sigma (for training)


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
    double slope;
    int n_input;
    int n_hidden;
    int n_output;
} NETWORK;


double my_square(double input);
double gauss();
double calc_output(UNIT_H unit);
void set_start_values(NETWORK *network);
void calc_network(NETWORK *network, double *in_data);
void mutate_network(NETWORK *network);