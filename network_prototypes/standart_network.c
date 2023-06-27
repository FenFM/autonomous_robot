//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   stndart_network.c                               //
//  Description:            simple neuronal net test                        //
//  Version:                0.4                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define PI 3.141
#define SIG_START 1
#define N_INPUT_LAYER  6
#define N_HIDDEN_LAYER 4
#define N_OUTPUT_LAYER 2
#define N_OF_CHILDS       50
#define N_OF_GENERATIONS 200


typedef struct unit{
    double netinput;
    double output;
    double weight;
    double sigma;
} UNIT;


typedef struct network{
    UNIT input_layer [N_INPUT_LAYER] ;
    UNIT hidden_layer[N_HIDDEN_LAYER];
    UNIT output_layer[N_OUTPUT_LAYER];
    int n_input;
    int n_hidden;
    int n_output;
} NETWORK;


typedef struct parent{
    NETWORK parent_network;
    double fitness;
} PARENT;


double my_square(double input){
    return input*input;
}


double gauss(){
    return sqrt(-2 * log(drand48())) * cos(2*PI*drand48());
}


double calc_output(double netinput, double weight){
    return weight / (1 + exp(netinput));
}


void calc_network(NETWORK *network, double *data){    
    // input layer
    for(int i=0; i<network->n_input; i++){
        network->input_layer[i].netinput = data[i];
        network->input_layer[i].output   = calc_output(network->input_layer[i].netinput, network->input_layer[i].weight);
    }

    // hidden layer
    for(int i=0; i<network->n_hidden; i++){
        network->hidden_layer[i].netinput = 0;
        for(int j=0; j<network->n_input; j++)
            network->hidden_layer[i].netinput += network->input_layer[j].output;
        network->hidden_layer[i].output = calc_output(network->hidden_layer[i].netinput, network->hidden_layer[i].weight);
    }

    // output layer
    for(int i=0; i<network->n_output; i++){
        network->output_layer[i].netinput = 0;
        for(int j=0; j<network->n_hidden; j++)
            network->output_layer[i].netinput += network->hidden_layer[j].output;
        network->output_layer[i].output = calc_output(network->output_layer[i].netinput, network->output_layer[i].weight);
    }
}


void set_start_values(NETWORK *network){
    for(int i=0; i<network->n_input; i++){
        network->input_layer[i].weight  = 1;
        network->input_layer[i].sigma   = SIG_START;
    }
    for(int j=0; j<network->n_hidden; j++){
        network->hidden_layer[j].weight = 1;
        network->hidden_layer[j].sigma  = SIG_START;
    }
    for(int k=0; k<network->n_output; k++){
        network->output_layer[k].weight = 1;
        network->output_layer[k].sigma  = SIG_START;
    }
}


double calc_fitness(NETWORK network, double *out_data){
    double fitness = 0;
    for(int i=0; i<network.n_output; i++){
        fitness += my_square(out_data[i] - network.output_layer[i].output);
    }
    return 0.5 * fitness;
}


int get_best_fitness(double *fitness, int n_child){
    int compare = 0;
    for(int i=1; i<n_child; i++)
        compare = (fitness[compare] < fitness[i])? compare : i;
    return compare;
}

// Error in here!
double calc_one_generation(NETWORK *network, double *in_data, double *out_data, int n_child){
    NETWORK child_network[n_child];
    double  fitness[n_child];
    int     best_fitness;

    for(int child=0; child<n_child; child++){
        // printf("%d\n", child);        
        // mutate every child
        for(int i=0; i<network->n_input;  i++){
            child_network[child].input_layer[i].weight  = network->input_layer[i].weight  + network->input_layer[i].sigma  * gauss();
        }
        for(int i=0; i<network->n_hidden; i++){
            child_network[child].hidden_layer[i].weight = network->hidden_layer[i].weight + network->hidden_layer[i].sigma * gauss();
        }
        for(int i=0; i<network->n_output; i++){
            child_network[child].output_layer[i].weight = network->output_layer[i].weight + network->output_layer[i].sigma * gauss();
        }

        // calculate child network
        calc_network(&child_network[child], in_data);

        // get the fitness of the child network
        fitness[child] = calc_fitness(child_network[child], out_data);
        printf("f(%d) = %f\n", child, fitness[child]);
    }

    // get the index (child) of the best fitness
    best_fitness = get_best_fitness(fitness, n_child);

    // overwrite the network with the child network with best parameters
    *network = child_network[best_fitness];

    return fitness[best_fitness];
}


// Main //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char**argv){

    NETWORK network;
    network.n_input  = N_INPUT_LAYER ;
    network.n_hidden = N_HIDDEN_LAYER;
    network.n_output = N_OUTPUT_LAYER;
    double fitness;

    // input data
    double in_data [6] = {10, 20, 30, 40, 50, 60};
    double out_data[2] = {0, 100};

    // set the layer values to a start value
    set_start_values(&network);

    for(int gen=0; gen<N_OF_GENERATIONS; gen++){
        fitness = calc_one_generation(&network, in_data, out_data, N_OF_CHILDS);
        printf("Fitness = %lf\n", fitness);
    }
    // printf("\nHERE\n");
    return 0;
}

// Main End //////////////////////////////////////////////////////////////////////////////////////////////////////////////////