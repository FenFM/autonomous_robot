//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   shared_memory_test.c                            //
//  Description:            creating and using shared memory                //
//  Version:                1.0                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


typedef struct sharedMemory{
    int data;
    double more_data;
} SHARED_MEMORY;


int main(int argc, char**argv){

    SHARED_MEMORY memSpace, *sharedMem;

    int shmID;
    key_t key;

    key = 1312;

    // create a parent and a child process
    if(fork() == 0){
        // create shared memory space and save the corresponding id the smhID
        if ((shmID = shmget(key, 2*sizeof(SHARED_MEMORY), IPC_CREAT | 0666)) == -1){
            perror("shmget 1");
            exit(1);
        }

        // attach memory space to memPNTR
        if ((sharedMem = (SHARED_MEMORY *) shmat(shmID, NULL, 0)) == (SHARED_MEMORY *) -1){
            perror("shmat 1");
            exit(1);
        }
        
        memSpace.data = 11;
        memSpace.more_data = 22.2;

        // copy the Information from memSpace to memPNTR
        memmove(sharedMem, &memSpace, sizeof(SHARED_MEMORY));

        /* Alternatively:
            sharedMem->data = 11;
            sharedMem->more_data = 22.2; 
        */
    }

    else{
        sleep(0.2);

        // acess shared memory space
        if ((shmID = shmget(key, 2*sizeof(SHARED_MEMORY), 0666)) == -1){
            perror("shmget 2");
            exit(1);
        }

        // attach memory space to memPNTR
        if ((sharedMem = (SHARED_MEMORY *) shmat(shmID, NULL, 0)) == (SHARED_MEMORY *) -1){
            perror("shmat 2");
            exit(1);
        }
        printf("%d\n%lf\n", sharedMem->data, sharedMem->more_data);
    }

    return 0;
}