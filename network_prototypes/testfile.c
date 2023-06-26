#include <stdio.h>


typedef struct var{
    int a;
    int b;
    struct var *pntr;
} VAR;

void plus_one(VAR *input){
    input->a ++;
    input->b ++;
}


int main(int argc, char**argv){

    VAR zahl;
    zahl.pntr = &zahl;
    zahl.a = 5;
    zahl.b = 10;

    printf("Zahl a = %d, Zahl b = %d\n", zahl.a, zahl.b);

    plus_one(zahl.pntr);
    printf("Zahl a = %d, Zahl b = %d\n", zahl.a, zahl.b);

    return 0;
}