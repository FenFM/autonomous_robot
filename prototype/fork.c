//////////////////////////////////////////////////////////////////////////////
//  Course:                 Nature Inspired Computing                       //
//  Lecturer:               Prof. Dr.-Ing. habil. Ralf Salomon              //
//  Author:                 B.Sc. Fenja Freitag                             //
//  Name:                   fork.c                                          //
//  Description:            testing different methods of fork()             //
//  Version:                0.1                                             //
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char**argv){
    if (argc != 2){
        printf("\nType: '%s number'!\n\n", argv[0]);
        exit(1);
    }
    int module = (int)argv[1][0] - 48;
    if(module < 0 || module > 9 || argv[1][1]){
        printf("\nNumber must be between 0 and 9!\n\n");
        exit(2);
    }

    int n_of_childs;
    int pid;
    int pipefd[2];

    switch(module){
        case 0:
            printf("Creating one child:\n");
            if(fork() == 0)
                printf("Hello, I'm child.\n");
            else
                printf("Hello, I'm parent.\n");
            break;


        case 1:
            printf("Type in number of childs: ");
            scanf("%d", &n_of_childs);

            while(n_of_childs){
                pid = fork();
                if(pid == 0)
                    break;
                n_of_childs--;
            }

            if(pid == 0)
                printf("Hello, I'm child #%d.\n", n_of_childs);
            else
                printf("Hello, I'm parent.\n");
            break;


        case 2:
            printf("Creating pipes:\n");
            if(pipe(pipefd)){
                printf("\nError with pipe!\n");
                exit(3);
            }
            if(fork() == 0){
                printf("Reading (Child) :\n");
            }
            else{
                printf("Writing (Parent):\n");
            }
            break;


        default:
            printf("Nothing is here ...\n");
            break; 
    }

}