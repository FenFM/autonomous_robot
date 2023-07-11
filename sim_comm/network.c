//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   network.c                                       //
//  Description:            network for the communicator                    //
//  Version:                1.0                                             //
//////////////////////////////////////////////////////////////////////////////

#include "network.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


double my_square(double input){
    return input*input;
}


double gauss(){
    return sqrt(-2 * log(drand48())) * cos(2*PI*drand48());
}


double calc_output(UNIT unit){
    return unit.weight / (1 + exp(unit.netinput - unit.activation));
}


void calc_network(NETWORK *network, double *in_data){    
    // input layer
    for(int i=0; i<network->n_input; i++){
        network->input_layer[i].netinput = in_data[i];
        network->input_layer[i].output   = calc_output(network->input_layer[i]);
    }

    // hidden layer one
    for(int i=0; i<(network->n_hidden_1/2); i++){
        network->hidden_layer_1[i].netinput = 0;
        for(int j=0; j<(network->n_input/2); j++)
            network->hidden_layer_1[i].netinput += network->input_layer[j].output;
        network->hidden_layer_1[i].output = calc_output(network->hidden_layer_1[i]);
    }
    for(int i=(network->n_hidden_1/2); i<network->n_hidden_1; i++){
        network->hidden_layer_1[i].netinput = 0;
        for(int j=(network->n_input/2); j<network->n_input; j++)
            network->hidden_layer_1[i].netinput += network->input_layer[j].output;
        network->hidden_layer_1[i].output = calc_output(network->hidden_layer_1[i]);
    }

    // hidden layer two
    for(int i=0; i<(network->n_hidden_2/4); i++){
        network->hidden_layer_2[i].netinput = 0;
        for(int j=0; j<(network->n_hidden_1/2); j++)
            network->hidden_layer_2[i].netinput += network->hidden_layer_1[j].output;
        network->hidden_layer_2[i].output = calc_output(network->hidden_layer_2[i]);
    }
    for(int i=(network->n_hidden_2/4); i<(network->n_hidden_2/2); i++){
        network->hidden_layer_2[i].netinput = 0;
        for(int j=0; j<(network->n_hidden_1/2); j++)
            network->hidden_layer_2[i].netinput += network->hidden_layer_1[j].output;
        network->hidden_layer_2[i].output = calc_output(network->hidden_layer_2[i]);
    }
    for(int i=(network->n_hidden_2/2); i<(network->n_hidden_2*3/4); i++){
        network->hidden_layer_2[i].netinput = 0;
        for(int j=(network->n_hidden_1/2); j<network->n_hidden_1; j++)
            network->hidden_layer_2[i].netinput += network->hidden_layer_1[j].output;
        network->hidden_layer_2[i].output = calc_output(network->hidden_layer_2[i]);
    }
    for(int i=(network->n_hidden_2*3/4); i<network->n_hidden_2; i++){
        network->hidden_layer_2[i].netinput = 0;
        for(int j=(network->n_hidden_1/2); j<network->n_hidden_1; j++)
            network->hidden_layer_2[i].netinput += network->hidden_layer_1[j].output;
        network->hidden_layer_2[i].output = calc_output(network->hidden_layer_2[i]);
    }

    // output layer
    /*
    for(int i=0; i<network->n_output; i++){
        network->output_layer[i].netinput = 0;
        for(int j=0; j<network->n_hidden_2; j++)
            network->output_layer[i].netinput += network->hidden_layer_2[j].output;
        network->output_layer[i].output = calc_output(network->output_layer[i]);
    }
    */

    network->output_layer[0].netinput = 0;
    network->output_layer[1].netinput = 0;

    network->output_layer[0].netinput += network->hidden_layer_2[0].output;
    network->output_layer[0].netinput += network->hidden_layer_2[1].output;
    network->output_layer[0].netinput += network->hidden_layer_2[4].output;
    network->output_layer[0].netinput += network->hidden_layer_2[5].output;

    network->output_layer[1].netinput += network->hidden_layer_2[2].output;
    network->output_layer[1].netinput += network->hidden_layer_2[3].output;
    network->output_layer[1].netinput += network->hidden_layer_2[6].output;
    network->output_layer[1].netinput += network->hidden_layer_2[7].output;

    network->output_layer[0].output = calc_output(network->output_layer[0]);
    network->output_layer[1].output = calc_output(network->output_layer[1]);
}


void mutate_network(NETWORK *network){
    for(int i=0; i<network->n_input;  i++){
        network->input_layer[i].weight      += network->input_layer[i].sigma  * gauss();
        network->input_layer[i].activation  += network->input_layer[i].sigma  * gauss();
    }
    for(int i=0; i<network->n_hidden_1; i++){
        network->hidden_layer_1[i].weight     += network->hidden_layer_1[i].sigma * gauss();
        network->hidden_layer_1[i].activation += network->hidden_layer_1[i].sigma * gauss();
    }
    for(int i=0; i<network->n_hidden_2; i++){
        network->hidden_layer_2[i].weight     += network->hidden_layer_2[i].sigma * gauss();
        network->hidden_layer_2[i].activation += network->hidden_layer_2[i].sigma * gauss();
    }
    for(int i=0; i<network->n_output; i++){
        network->output_layer[i].weight     += network->output_layer[i].sigma * gauss();
        network->output_layer[i].activation += network->output_layer[i].sigma * gauss();
    }    
}


void set_start_values(NETWORK *network){
    for(int i=0; i<network->n_input; i++){
        network->input_layer[i].weight  = 1;
        network->input_layer[i].activation  = 1;
        network->input_layer[i].sigma   = SIG_START;
    }
    for(int j=0; j<network->n_hidden_1; j++){
        network->hidden_layer_1[j].weight = 1;
        network->hidden_layer_1[j].activation = 1;
        network->hidden_layer_1[j].sigma  = SIG_START;
    }
    for(int j=0; j<network->n_hidden_2; j++){
        network->hidden_layer_2[j].weight = 1;
        network->hidden_layer_2[j].activation = 1;
        network->hidden_layer_2[j].sigma  = SIG_START;
    }
    for(int k=0; k<network->n_output; k++){
        network->output_layer[k].weight = 1;
        network->output_layer[k].activation = 1;
        network->output_layer[k].sigma  = SIG_START;
    }
}


double calc_fitness(NETWORK network, double *out_data){
    double fitness = 0;
    for(int i=0; i<network.n_output; i++){
        fitness += my_square(out_data[i] - network.output_layer[i].output);
    }
    return fitness;
}


int get_best_fitness(double *fitness, int n_child){
    int compare = 0;
    for(int i=1; i<n_child; i++)
        compare = (fitness[compare] < fitness[i])? compare : i;
    return compare;
}


double calc_one_generation(NETWORK *network, double *in_data, double *out_data, int n_data, int n_child){
    NETWORK child_network[n_child];
    double  fitness[n_child];
    int     best_fitness;

    for(int child=0; child<n_child; child++){
        // copy main network values to child network
        child_network[child] = *network;
        
        // mutate child network
        mutate_network(&child_network[child]);

        // calculate child network and get part of the fitness
        fitness[child] = 0;
        for(int set=0; set<n_data; set++){
            calc_network(&child_network[child], in_data+(set*6));
            fitness[child] += calc_fitness(child_network[child], out_data+(set*2));
        }
        fitness[child] *= 0.5;
    }

    // get the index (child) of the best fitness
    best_fitness = get_best_fitness(fitness, n_child);

    // overwrite the network with the child network with best parameters
    *network = child_network[best_fitness];

    return fitness[best_fitness];
}