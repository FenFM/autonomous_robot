//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   sim_communicator.c                              //
//  Description:            communicationg with simulation via shared       //
//                          memory                                          //
//  Version:                0.6                                             //
//////////////////////////////////////////////////////////////////////////////

#include "rbs_network.h"


typedef struct my_len{
   double len;
   double old_x;
   double old_y;
} MY_LEN;

typedef struct memspace{
    short int comm_flag;
    short int train_flag;
    short int IR_Distance[6];
    short int Motor_Value[2];
    MY_LEN distance;
} MemSpace;


int main(int argc, char**argv){
    // initialize srand for randomizing childs
    time_t t;
    srand48((unsigned) time(&t));


    MemSpace robot_values, *sharedMem;

    int shmID;
    key_t key = 42071;


   if ((shmID = shmget(key, sizeof(MemSpace), 0666)) == -1){
      perror("shmget");
      exit(1);
   }

   if ((sharedMem = (MemSpace *) shmat(shmID, NULL, 0)) == (MemSpace *) -1){
      perror("shmat");
      exit(1);
   }

    NETWORK network[MAX_CHILD];
    int current_child = 0;

    // initialize and randomize all child networks
    for(int i=0; i<MAX_CHILD; i++){
        set_start_values(&network[i]);
        mutate_network(&network[i]);
    }


    while(1){
        while(sharedMem->comm_flag == 0){sleep(0.0001);}
            if(sharedMem->train_flag == 1){
                sharedMem->train_flag = 0;
                network[current_child].fitness = sharedMem->distance.len;
                printf("%d -> %lf\n", current_child, network[current_child].fitness);
                generation_step_forward(&network, &current_child, MAX_CHILD);
                sharedMem->distance.len = 0;
            }

            calc_network(&network[current_child], (double *) sharedMem->IR_Distance);
            
            sharedMem->Motor_Value[0] = (short int) network[current_child].output_layer[0].output;
            sharedMem->Motor_Value[1] = (short int) network[current_child].output_layer[1].output;
            
            sharedMem->comm_flag = 0;
    }

    return 0;
}



