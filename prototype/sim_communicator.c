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
    short int work_flag;
    short int IR_Distance[6];
    short int Motor_Value[2];
} MemSpace;


int main(int argc, char**argv){

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


    while(1){
        if(sharedMem->work_flag == 0){
            sharedMem->work_flag = 1;

            // get the values from the shared memory
            memmove(&robot_values, sharedMem, sizeof(MemSpace));

            robot_values.work_flag = 0;

            // Read IR Values
            printf("IR Value 1 = %d\n", robot_values.IR_Distance[1]);

            // copy the values to the shared memory
            memmove(sharedMem, &robot_values, sizeof(MemSpace));

            sleep(1);
        }
    }

    return 0;
}



