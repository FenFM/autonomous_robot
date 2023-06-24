//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   msg_writer.c                                    //
//  Description:            writing to a message                            //
//  Version:                0.1                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>


typedef struct msg_buffer{
    long msg_type;
    char msg_text[100];
} MSG_BUFFER;


int main(int argc, char**argv){
    
    key_t key;
    int msgid;
    int counter = 0;
    MSG_BUFFER message;
    char msg[100] = "Hallo Welt";

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.msg_type = 1;

    printf("Writing Data: ");
    while(1){
        strcpy(message.msg_text, msg);
        msgsnd(msgid, &message, sizeof(message), 0);
        printf("Data send is : %s\n", message.msg_text, counter);
        counter ++;
        sleep(1);
    }

    return 0;
}



