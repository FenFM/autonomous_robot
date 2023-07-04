#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>


typedef struct msg_buffer{
    long msg_type;
    short int IR_Distance[6];
} MSG_BUFFER;


int main(int argc, char**argv){
    
    int msgid_1, msgid_2;
    MSG_BUFFER message;

    msgid_1 = msgget(ftok("progfile", 65), 0666 | IPC_CREAT);
    msgid_2 = msgget(ftok("progfile", 66), 0666 | IPC_CREAT);
    
    while(1){
        msgctl(msgid_1, IPC_RMID, NULL);
        msgctl(msgid_2, IPC_RMID, NULL);
    }

    return 0;
}



