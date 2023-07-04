//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   msg_reader.c                                    //
//  Description:            reading from a message                          //
//  Version:                1.0                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>


typedef struct msg_buffer{
    long msg_type;
    short int IR_Distance[6];
    short int Motor_Value[2];
} MSG_BUFFER;


int main(int argc, char**argv){
    
    key_t key_1, key_2;
    int msgid_1, msgid_2;
    MSG_BUFFER message;

    key_1 = ftok("progfile", 65);
    msgid_1 = msgget(key_1, 0666 | IPC_CREAT);

    key_2 = ftok("progfile", 66);
    msgid_2 = msgget(key_2, 0666 | IPC_CREAT);


    if(fork() == 0){
        while(1){
            msgrcv(msgid_1, &message, sizeof(message), 1, 0);
            printf("Data Recieved is: %d\n", message.IR_Distance[1]);
        }
    }

    else{
        message.msg_type = 1;
        message.Motor_Value[0] = 0;
        message.Motor_Value[1] = 0;

        while(1){
            msgsnd(msgid_2, &message, sizeof(message), 0);
            message.Motor_Value[0]++;
        }

    }

    msgctl(msgid_1, IPC_RMID, NULL);
    msgctl(msgid_2, IPC_RMID, NULL);
    return 0;
}



