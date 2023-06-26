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


typedef struct msg_buffer{
    long msg_type;
    char msg_text[100];
    int  msg_info;
} MSG_BUFFER;


int main(int argc, char**argv){
    
    key_t key;
    int msgid;
    MSG_BUFFER message;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    while(1){
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        printf("Data Recieved is: %s #%d\n", message.msg_text, message.msg_info);
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}



