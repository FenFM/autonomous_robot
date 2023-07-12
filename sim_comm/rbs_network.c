//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   rbs_network.c                                   //
//  Description:            simple rbs neuronal net test                    //
//  Version:                0.6                                             //
//////////////////////////////////////////////////////////////////////////////

#include"rbs_network.h"


double my_square(double input){
    return input*input;
}


double gauss(){
    return sqrt(-2 * log(drand48())) * cos(2*PI*drand48());
}


double calc_output(UNIT_H unit){
    return exp( -(my_square(unit.omega-unit.netinput) ) / unit.sigma );
}


void set_start_values(NETWORK *network){
    network->n_input  = N_INPUT_LAYER;
    network->n_hidden = N_HIDDEN_LAYER;
    network->n_output = N_OUTPUT_LAYER;
    network->slope    = SLOPE_START;
    for(int i=0; i<network->n_input; i++)
        network->input_layer[i].weight = 1;
    for(int i=0; i<network->n_hidden; i++){
        network->hidden_layer[i].activation = 4;
        network->hidden_layer[i].omega = 1;
        network->hidden_layer[i].sigma = 1;
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


void mutate_network(NETWORK *network){
    if(gauss() <= 0.33)
        network->slope *= 0.75;
    else if(gauss() <= 0.66)
        network->slope *= 1.33;

    for(int i=0; i<network->n_input; i++)
        network->input_layer[i].weight      += network->slope * gauss();
    for(int i=0; i<network->n_hidden; i++){
        network->hidden_layer[i].activation += network->slope * gauss();
        network->hidden_layer[i].omega      += network->slope * gauss();
        network->hidden_layer[i].sigma      += network->slope * gauss();
    }
}


void generation_step_forward(NETWORK *network, int *child, int max_child){
    NETWORK parent_network;
    int best_fitness;

    if(max_child == ++(*child)){        
        *child = 0;

        best_fitness = get_best_fitness(&network[0], max_child);
        parent_network = network[best_fitness];

        for(int i=0; i<max_child; i++){
            network[i] = parent_network;
            mutate_network(&network[i]);
        }

        printf("\nf(%d) = %lf\n", best_fitness, parent_network.fitness);
    }
}


int get_best_fitness(NETWORK *network, int max_child){
    int compare = 0;
    for(int i=1; i<max_child; i++)
        compare = (network[compare].fitness > network[i].fitness)? compare : i;
    return compare;
}