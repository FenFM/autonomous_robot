//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   pipe_reader.c                                   //
//  Description:            reading from a pipe                             //
//  Version:                0.1                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define buffsize 64


int main(int argc, char**argv){

    FILE *fp;
    
    char *myfifo = "/tmp/myfifo";
    char message[buffsize];


    while(1){
        read(pipefd[0], message, buffsize);
        printf("%s\n" , message);    
    }

    return 0;
}



