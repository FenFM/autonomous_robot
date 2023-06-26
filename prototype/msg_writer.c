//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   msg_writer.c                                    //
//  Description:            writing to a message                            //
//  Version:                1.0                                             //
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
    int  msg_info;
} MSG_BUFFER;


int main(int argc, char**argv){
    
    key_t key;
    int msgid;
    MSG_BUFFER message;
    char *msg = "Hallo Welt";

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.msg_type = 1;
    message.msg_info = 0;
    strcpy(message.msg_text, msg);

    printf("Writing Data: ");
    while(1){
        msgsnd(msgid, &message, sizeof(message), 0);
        printf("Data send is : %d\n", message.msg_info);
        message.msg_info ++;
        sleep(1);
    }

    return 0;
}



