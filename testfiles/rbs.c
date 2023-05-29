//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   rbs.c                                           //
//  Description:            simple rbs neuronal-net                         //
//  Version:                0.1                                             //
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
    return exp( -(((omega-input)/(sigma)) * ((omega-input)/(sigma))) );
}


void calculate_network(CELL *neuron, double *data_feed){
        double hidden_sum = 0;
        double output_sum = 0;
        // input layer
        for(int i=0; i<6; i++){
            neuron[i].input  = data_feed[i];
            neuron[i].output = output_calculator(neuron[i].input, neuron[i].omega, neuron[i].sigma);
            hidden_sum += neuron[i].output;
        }
        // hidden layer
        for(int i=6; i<10; i++){
            neuron[i].input  = hidden_sum;
            neuron[i].output = output_calculator(neuron[i].input, neuron[i].omega, neuron[i].sigma);
            output_sum += neuron[i].output;
        }
        // output layer
         for(int i=10; i<12; i++){
            neuron[i].input  = output_sum;
            neuron[i].output = output_calculator(neuron[i].input, neuron[i].omega, neuron[i].sigma);
        }       
}


double calculate_soft_max(CELL *neuron){
    double sum_1 = 0;
    double sum_2 = 0;
    for(int i=0; i<6; i++){
        sum_1 += neuron[i].activation * neuron[i].output;
        sum_2 += neuron[i].output;
    }
    return sum_1/sum_2;
}


// Main ////////////////////////////////////////////////////////////////////////////

int main(int argc, char**argv){
    double soft_max;
    CELL neuron[12];

    // defining the input-data
    double data_feed[6] = {1, 2, 3, 4, 5, 6};

    // initializing the neurons
    for(int i=0; i<12; i++){
        neuron[i].activation = i;
        neuron[i].omega = i+1;
        neuron[i].sigma = i+2;
    }

    calculate_network(neuron, data_feed);
    soft_max = calculate_soft_max(neuron);

    printf("Soft Max = %lf\n", soft_max);

    return 0;
}

// Main End ////////////////////////////////////////////////////////////////////////