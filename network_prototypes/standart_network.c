//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   stndart_network.c                               //
//  Description:            simple neuronal net test                        //
//  Version:                0.1                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define PI 3.141


typedef struct unit{
    double netinput;
    double output;
    double sigma;
} UNIT;


double my_square(double input){
    return input*input;
}


double calc_output(double netinput, double sigma){
    return exp( -my_square(sigma-netinput) / sigma );
}


void calc_network(UNIT *input_layer, UNIT *hidden_layer, UNIT *output_layer, double *data){
    
    // input layer
    for(int i=0; i<6; i++){
        input_layer[i].netinput = data[i];
        input_layer[i].output   = calc_output(input_layer[i].netinput, input_layer[i].sigma);
    }

    // hidden layer
    for(int i=0; i<4; i++){
        hidden_layer[i].netinput = 0;
        for(int j=0; j<6; j++)
            hidden_layer[i].netinput += input_layer[j].output;
        hidden_layer[i].output   = calc_output(hidden_layer[i].netinput, hidden_layer[i].sigma);
    }

    // output layer
    for(int i=0; i<2; i++){
        output_layer[i].netinput = 0;
        for(int j=0; j<4; j++)
            output_layer[i].netinput += hidden_layer[j].output;
        output_layer[i].output   = calc_output(output_layer[i].netinput, output_layer[i].sigma);
    }
}