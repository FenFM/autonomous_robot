//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   network.c                                       //
//  Description:            network for the communicator                    //
//  Version:                1.0                                             //
//////////////////////////////////////////////////////////////////////////////

#include "network.h"


double my_square(double input){
    return input*input;
}


double gauss(){
    return sqrt(-2 * log(drand48())) * cos(2*PI*drand48());
}


double calc_output(UNIT unit){
    // return unit.weight / (1 + exp(unit.netinput - unit.activation));
    return unit.weight * tanh(unit.netinput - unit.activation);
}


void calc_network(NETWORK *network, int *in_data){    
    // input layer
    for(int i=0; i<network->n_input; i++){
        network->input_layer[i].netinput = (double) in_data[i];
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
    network->output_layer[0].netinput = 0;
    for(int i=0; i<network->n_hidden_2/4; i++)
        network->output_layer[0].netinput += network->hidden_layer_2[i].output;

    for(int i=network->n_hidden_2/2; i<network->n_hidden_2*3/4; i++)
        network->output_layer[0].netinput += network->hidden_layer_2[i].output;


    network->output_layer[1].netinput = 0;
    for(int i=network->n_hidden_2/4; i<network->n_hidden_2/2; i++)
        network->output_layer[1].netinput += network->hidden_layer_2[i].output;

    for(int i=network->n_hidden_2*3/4; i<network->n_hidden_2; i++)
        network->output_layer[1].netinput += network->hidden_layer_2[i].output;

    network->output_layer[0].output = calc_output(network->output_layer[0]);
    network->output_layer[1].output = calc_output(network->output_layer[1]);    
}


void mutate_network(NETWORK *network){
    if(gauss() <= 0.33)
        network->slope *= 0.9;
    else if(gauss() <= 0.66)
        network->slope *= 1.1;

    for(int i=0; i<network->n_input;  i++){
        network->input_layer[i].weight      += network->slope  * gauss();
        network->input_layer[i].activation  += network->slope  * gauss();
    }
    for(int i=0; i<network->n_hidden_1; i++){
        network->hidden_layer_1[i].weight     += network->slope * gauss();
        network->hidden_layer_1[i].activation += network->slope * gauss();
    }
    for(int i=0; i<network->n_hidden_2; i++){
        network->hidden_layer_2[i].weight     += network->slope * gauss();
        network->hidden_layer_2[i].activation += network->slope * gauss();
    }
    for(int i=0; i<network->n_output; i++){
        network->output_layer[i].weight     += network->slope * gauss();
        network->output_layer[i].activation += network->slope * gauss();
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


void set_start_values(NETWORK *network){
    network->n_input    = N_INPUT_LAYER;
    network->n_hidden_1 = N_HIDDEN_LAYER_1;
    network->n_hidden_2 = N_HIDDEN_LAYER_2;
    network->n_output   = N_OUTPUT_LAYER;
    network->slope      = SLOPE_START;

    int inc = 10;

    for(int i=0; i<network->n_input; i++){
        network->input_layer[i].weight  = inc * gauss();
        network->input_layer[i].activation  = inc * gauss();
    }
    for(int j=0; j<network->n_hidden_1; j++){
        network->hidden_layer_1[j].weight = inc * gauss();
        network->hidden_layer_1[j].activation = inc * gauss();
    }
    for(int j=0; j<network->n_hidden_2; j++){
        network->hidden_layer_2[j].weight = inc * gauss();
        network->hidden_layer_2[j].activation = inc * gauss();
    }
    for(int k=0; k<network->n_output; k++){
        network->output_layer[k].weight = inc * gauss();
        network->output_layer[k].activation = inc * gauss();
    }
}


int get_best_fitness(NETWORK *network, int max_child){
    int compare = 0;
    for(int i=1; i<max_child; i++)
        compare = (network[compare].fitness > network[i].fitness)? compare : i;
    return compare;
}


void value_unit(NETWORK *network, MOV_VAL *motor){
    // write the current motor value in the motor_array
    motor->l_motor_arr[motor->arr_cntr] = (short int) network->output_layer[0].output;
    motor->r_motor_arr[motor->arr_cntr] = (short int) network->output_layer[1].output;

    // calculate the avarage motor values
    motor->l_motor_avarage = 0;
    motor->r_motor_avarage = 0;
    for(int i=0; i<255; i++){
        motor->l_motor_avarage += motor->l_motor_arr[i];
        motor->r_motor_avarage += motor->r_motor_arr[i];
    }
    motor->l_motor_avarage /= motor->avarage_cntr;
    motor->r_motor_avarage /= motor->avarage_cntr;
    motor->comb_avarage = (motor->l_motor_avarage + motor->r_motor_avarage) / 2;

    // the turn value detects if the robot is turning left, right or not
    motor->turn_value[motor->arr_cntr] = (motor->l_motor_avarage - motor->r_motor_avarage) / (motor->l_motor_avarage + motor->r_motor_avarage);
    motor->turn_value_avarage = 0;
    for(int i=0; i<255; i++)
        motor->turn_value_avarage += motor->turn_value[i];
    motor->turn_value_avarage /= motor->avarage_cntr;

    // make sure the motor does not oversteer or understeer
    if((network->output_layer[0].output > 10.0) && (network->output_layer[1].output > 10.0)){
        network->output_layer[0].output = 10.0;
        network->output_layer[1].output = 10.0;
    }

    if((network->output_layer[0].output <= - 2.0) && (network->output_layer[1].output <= - 2.0)){
        network->output_layer[0].output = 0.0;
        network->output_layer[1].output = 0.0;
    }

    if(check_netinput(*network, 50)){
        network->output_layer[0].output = motor->comb_avarage;
        network->output_layer[1].output = motor->comb_avarage;
    }

    motor->arr_cntr     += (motor->arr_cntr     == 255)? -255 : 1;
    motor->avarage_cntr += (motor->avarage_cntr == 256)?    0 : 1;
}


short int check_netinput(NETWORK network, short int val){
    for(int i=0; i<6; i++){
        if(network.input_layer[i].netinput > val)
            return 0;
        else
            return 1;
    }
} 


void set_mov_val(MOV_VAL *motor){
    motor->arr_cntr     = 0;
    motor->avarage_cntr = 1;
    motor->l_motor_avarage = 0; 
    motor->r_motor_avarage = 0;
    motor->turn_value_avarage = 0;
    for(int i=0; i<255; i++){
        motor->turn_value[i]  = 0;
        motor->l_motor_arr[i] = 0;
        motor->r_motor_arr[i] = 0;
    }
}


void print_network(NETWORK network){
    printf("\n");
    for(int i=0; i<network.n_input; i++){
        printf("Input %2d: A = %2.3lf , W = %2.3lf , I = %2.3lf , O = %2.3lf\n", i, network.input_layer[i].activation, network.input_layer[i].weight, network.input_layer[i].netinput, network.input_layer[i].output);
    }
    for(int i=0; i<network.n_output; i++){
        printf("Input %2d: A = %2.3lf , W = %2.3lf , I = %2.3lf , O = %2.3lf\n", i, network.output_layer[i].activation, network.output_layer[i].weight, network.output_layer[i].netinput, network.output_layer[i].output);
    }
    printf("\n");
}