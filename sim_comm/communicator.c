//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   sim_communicator.c                              //
//  Description:            communicationg with simulation via shared       //
//                          memory                                          //
//  Version:                1.0                                             //
//////////////////////////////////////////////////////////////////////////////

#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

/*
typedef struct memspace{
    short int work_flag;
    short int IR_Distance[6];
    short int Motor_Value[2];
} MemSpace;
*/

int main(int argc, char**argv){

    time_t t;
    srand48((unsigned) time(&t));


    MemSpace robot_values, *sharedMem;

    int shmID;
    key_t key = 42069;


   if ((shmID = shmget(key, 2*sizeof(MemSpace), 0666)) == -1){
      perror("shmget");
      exit(1);
   }

   if ((sharedMem = (MemSpace *) shmat(shmID, NULL, 0)) == (MemSpace *) -1){
      perror("shmat");
      exit(1);
   }

    double fitness;
    NETWORK network;
    network.n_input    = N_INPUT_LAYER;
    network.n_hidden_1 = N_HIDDEN_LAYER_1;
    network.n_hidden_2 = N_HIDDEN_LAYER_2;
    network.n_output   = N_OUTPUT_LAYER;
    set_start_values(&network);

    double in_data [N_OF_DATA_SETS][6] = {{0, 0, 0, 0, 0, 0}, {0, 1000, 1000, 1000, 1000, 0}};
    double out_data[N_OF_DATA_SETS][2] = {{6, 6}, {-6, -6}};

    for(int gen=0; gen<N_OF_GENERATIONS; gen++){
        fitness = calc_one_generation(&network, in_data, out_data, N_OF_DATA_SETS, N_OF_CHILDS);
    }
    printf("Fitness = %lf\n", fitness);


    while(1){
        while(sharedMem->comm_flag == 0){sleep(0.0001);}
            calc_network(&network, (double *) sharedMem->IR_Distance);
            
            sharedMem->Motor_Value[0] = network.output_layer[0].output;
            sharedMem->Motor_Value[1] = network.output_layer[1].output;
            
            sharedMem->comm_flag = 0;
    }

    return 0;
}



