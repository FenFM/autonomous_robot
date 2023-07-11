//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   sim_communicator.c                              //
//  Description:            communicationg with simulation via shared       //
//                          memory                                          //
//  Version:                1.0                                             //
//////////////////////////////////////////////////////////////////////////////

#include "rbs_network.h"


typedef struct memspace{
    short int comm_flag;
    short int IR_Distance[6];
    short int Motor_Value[2];
} MemSpace;


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

    NETWORK network;
    network.n_input  = N_INPUT_LAYER;
    network.n_hidden = N_HIDDEN_LAYER;
    network.n_output = N_OUTPUT_LAYER;
    set_start_values(&network);


    while(1){
        while(sharedMem->comm_flag == 0){sleep(0.0001);}
            calc_network(&network, (double *) sharedMem->IR_Distance);
            
            sharedMem->Motor_Value[0] = network.output_layer[0].output;
            sharedMem->Motor_Value[1] = network.output_layer[1].output;
            
            sharedMem->comm_flag = 0;
    }

    return 0;
}



