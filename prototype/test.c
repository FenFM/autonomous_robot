#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t id = fork();
    printf("pid_t = %d, getpgid = %d, other = %d\n", id, getpgid(id), getpgrp());
    // printf("hanlo\n");
}
