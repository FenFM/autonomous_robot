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
        network->input_layer[i].weight = 10 * gauss();
    for(int i=0; i<network->n_hidden; i++){
        network->hidden_layer[i].activation = 10 * gauss();
        network->hidden_layer[i].omega = 10 * gauss();
        network->hidden_layer[i].sigma = 10 * gauss();
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
        network->slope *= 0.95;
    else if(gauss() <= 0.66)
        network->slope *= 1.05;

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

        for(int j=0; j<max_child; j++){
            network[j] = parent_network;
            mutate_network(&network[j]);
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
    // DEBUG printf("LM = %lf\nRM = %lf\nAV = %d\n", motor->l_motor_avarage, motor->r_motor_avarage, motor->comb_avarage);

    // the turn value detects if the robot is turning left, right or not
    motor->turn_value[motor->arr_cntr] = (motor->l_motor_avarage - motor->r_motor_avarage) / (motor->l_motor_avarage + motor->r_motor_avarage);
    motor->turn_value_avarage = 0;
    for(int i=0; i<255; i++)
        motor->turn_value_avarage += motor->turn_value[i];
    motor->turn_value_avarage /= motor->avarage_cntr;

    if(check_netinput(*network, 10)){
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