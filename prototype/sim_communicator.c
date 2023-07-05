//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   sim_communicator.c                              //
//  Description:            communicationg with simulation via shared       //
//                          memory                                          //
//  Version:                1.0                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


typedef struct memspace{
    long msg_type;
    short int IR_Distance[6];
    short int Motor_Value[2];
} MemSpace;


int main(int argc, char**argv){

    MemSpace robot_values, *robot_pointer;

    int shmID;
    key_t key = 42069;

    int distance_threshold[6] = {200, 400, 600, 600, 400, 200};


   if ((shmID = shmget(key, 2*sizeof(MemSpace), 0666)) == -1){
      perror("shmget");
      exit(1);
   }

   if ((robot_pointer = (MemSpace *) shmat(shmID, NULL, 0)) == (MemSpace *) -1){
      perror("shmat");
      exit(1);
   }


    while(1){
        // get the values from the shared memory
        memmove(&robot_values, robot_pointer, sizeof(MemSpace));

        // Read IR Values
        printf("IR Value 1 = %d\n", robot_values.IR_Distance[1]);

        // copy the values to the shared memory
        memmove(robot_pointer, &robot_values, sizeof(MemSpace));

        sleep(0.1);
    }

    return 0;
}



