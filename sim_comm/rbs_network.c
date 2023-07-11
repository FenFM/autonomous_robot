//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   rbs_network.c                                   //
//  Description:            simple rbs neuronal net test                    //
//  Version:                0.5                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define PI 3.141
#define N_INPUT_LAYER  6        // don't change
#define N_HIDDEN_LAYER 16       // must be devidable by 4
#define N_OUTPUT_LAYER 2        // don't change


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
    int n_input;
    int n_hidden;
    int n_output;
} NETWORK;


double my_square(double input){
    return input*input;
}


double calc_output(UNIT_H unit){
    return exp( -(my_square(unit.omega-unit.netinput) ) / unit.sigma );
}


void set_start_values(NETWORK *network){
    for(int i=0; i<network->n_input; i++)
        network->input_layer[i].weight = 1;
    for(int i=0; i<network->n_hidden; i++){
        network->hidden_layer[i].activation = i;
        network->hidden_layer[i].omega = i;
        network->hidden_layer[i].sigma = i;
    }
}


void calc_network(NETWORK *network, double *in_data){
        // input layer
        for(int i=0; i<network->n_input; i++){
            network->input_layer[i].netinput = in_data[i];
            network->input_layer[i].output   = network->input_layer[i].netinput * network->input_layer[i].weight;
        }


        // hidden layer
        for(int i=0; i<network->n_hidden/2; i++){
            network->hidden_layer[i].netinput = 0;
            for(int j=0; j<network->n_input/2; j++)
                network->hidden_layer[i].netinput += network->input_layer[j].output;
            network->hidden_layer[i].output = calc_output(network->hidden_layer[i]);
        }

       for(int i=network->n_hidden/2; i<network->n_hidden; i++){
            network->hidden_layer[i].netinput = 0;
            for(int j=network->n_input/2; j<network->n_input; j++)
                network->hidden_layer[i].netinput += network->input_layer[j].output;
            network->hidden_layer[i].output = calc_output(network->hidden_layer[i]);
        }


        // output_layer
        network->output_layer[0].upper_input = 0;
        network->output_layer[0].lower_input = 0;
        for(int i=0; i<network->n_hidden/4; i++){
            network->output_layer[0].upper_input += network->hidden_layer[i].output * network->hidden_layer[i].activation;
            network->output_layer[0].lower_input += network->hidden_layer[i].output;
        }
        for(int i=network->n_hidden/2; i<network->n_hidden*3/4; i++){
            network->output_layer[0].upper_input += network->hidden_layer[i].output * network->hidden_layer[i].activation;
            network->output_layer[0].lower_input += network->hidden_layer[i].output;
        }

        network->output_layer[1].upper_input = 0;
        network->output_layer[1].lower_input = 0;
        for(int i=network->n_hidden/4; i<network->n_hidden/2; i++){
            network->output_layer[1].upper_input += network->hidden_layer[i].output * network->hidden_layer[i].activation;
            network->output_layer[1].lower_input += network->hidden_layer[i].output;
        }
        for(int i=network->n_hidden*3/4; i<network->n_hidden; i++){
            network->output_layer[1].upper_input += network->hidden_layer[i].output * network->hidden_layer[i].activation;
            network->output_layer[1].lower_input += network->hidden_layer[i].output;
        }

        network->output_layer[0].output = network->output_layer[0].upper_input / network->output_layer[0].lower_input;
        network->output_layer[1].output = network->output_layer[1].upper_input / network->output_layer[1].lower_input;
}


// Main ////////////////////////////////////////////////////////////////////////////

int main(int argc, char**argv){
    
    NETWORK network;
    network.n_input  = N_INPUT_LAYER;
    network.n_hidden = N_HIDDEN_LAYER;
    network.n_output = N_OUTPUT_LAYER;

    set_start_values(&network);

    // defining the input-data: the input values of the infrared-sensors
    double data_feed[6] = {1, 1, 1, 1, 1, 100};


    calc_network(&network, data_feed);

    printf("Left Motor  = %lf\n", network.output_layer[0].output);
    printf("Right Motor = %lf\n", network.output_layer[1].output);

    return 0;
}

// Main ////////////////////////////////////////////////////////////////////////////