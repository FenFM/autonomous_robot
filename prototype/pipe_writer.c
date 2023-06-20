//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   pipe_writer.c                                   //
//  Description:            writing to a pipe                               //
//  Version:                0.1                                             //
//////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define buffsize 64



int main(int argc, char**argv){
    
    int pipefd[2] = "a.out";
    char message[buffsize] = "Hello, some text!\n";


    while(1){
        write(pipefd[1], message, buffsize);
        printf("%s\n"  , message);
        sleep(0.5);
    }

    return 0;
}