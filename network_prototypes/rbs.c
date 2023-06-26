//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   rbs.c                                           //
//  Description:            simple rbs neuronal net test                    //
//  Version:                0.3                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define PI 3.141


typedef struct cell{
    double input;
    double omega;
    double sigma;
    double activation;
    double output;
} CELL;


double output_calculator(double input, double omega, double sigma){
    return exp( -(((omega-input)) * ((omega-input))) / sigma );
}


void calculate_network(CELL *neuron, double *data_feed){
        double hidden_sum_1 = 0;
        double hidden_sum_2 = 0;

        // input layer
        for(int i=0; i<3; i++){
            neuron[i].input  = data_feed[i];
            neuron[i].output = neuron[i].input * neuron[i].omega;
            hidden_sum_1 += neuron[i].output;
        }
        for(int i=3; i<6; i++){
            neuron[i].input  = data_feed[i];
            neuron[i].output = neuron[i].input * neuron[i].omega;
            hidden_sum_2 += neuron[i].output;
        }  

        // hidden layer
        for(int i=6; i<8; i++){
            neuron[i].input  = hidden_sum_1;
            neuron[i].output = output_calculator(neuron[i].input, neuron[i].omega, neuron[i].sigma);
        }
        for(int i=8; i<10; i++){
            neuron[i].input  = hidden_sum_2;
            neuron[i].output = output_calculator(neuron[i].input, neuron[i].omega, neuron[i].sigma);
        }

        // output layer
        neuron[10].input = neuron[6].output + neuron[8].output;
        neuron[11].input = neuron[7].output + neuron[9].output;

        // Output via Softmax
        neuron[10].output = ((neuron[6].activation*neuron[6].output) + (neuron[8].activation*neuron[8].output)) / (neuron[10].input);
        neuron[11].output = ((neuron[7].activation*neuron[7].output) + (neuron[9].activation*neuron[9].output)) / (neuron[11].input);

}


// Main ////////////////////////////////////////////////////////////////////////////

int main(int argc, char**argv){
    double soft_max;
    CELL neuron[12];

    // defining the input-data: the input values of the infrared-sensors
    double data_feed[6] = {8, 8, 8, 8, 8, 10};

    // initializing the neurons
    for(int i=0; i<12; i++){
        neuron[i].activation = 1;
        neuron[i].omega = 1;
        neuron[i].sigma = 2;
    }
    neuron[6].activation = -1;
    neuron[7].activation = 5;
    neuron[8].activation = 5;
    neuron[9].activation = -1;
    neuron[6].omega = 5;
    neuron[7].omega = 0;
    neuron[8].omega = 0;
    neuron[9].omega = 5;

    calculate_network(neuron, data_feed);

    printf("Left Motor  = %lf\n", neuron[10].output);
    printf("Right Motor = %lf\n", neuron[11].output);

    return 0;
}

// Main ////////////////////////////////////////////////////////////////////////////