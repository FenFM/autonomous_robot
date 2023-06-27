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

void minus_one(VAR input){
    input.a --;
    input.b --;
}

int main(int argc, char**argv){

    VAR zahl;
    zahl.pntr = &zahl;
    zahl.a = 5;
    zahl.b = 10;

    printf("Zahl a = %d, Zahl b = %d\n", zahl.a, zahl.b);
    plus_one(&zahl);
    printf("Zahl a = %d, Zahl b = %d\n", zahl.a, zahl.b);
    minus_one(zahl);
    printf("Zahl a = %d, Zahl b = %d\n", zahl.a, zahl.b);
    printf("\n");

    // int var[1] entspricht int *var;

    VAR block[2];
    block[0].a = 20; // same as block->a = 20;
    block[0].b = 40;
    block[0].pntr = &block[0];
    block[1].a = 0;
    block[1].b = 2;

    printf("Zahl a = %d, Zahl b = %d\n", block[0].a, block[0].b);
    plus_one( block);
    plus_one(&block[0]);
    plus_one( block[0].pntr);
    printf("Zahl a = %2d, Zahl b = %2d\n", block[0].a, block[0].b);
    printf("Zahl a = %2d, Zahl b = %2d\n", block[1].a, block[1].b);
    // printf("Zahl a = %d, Zahl b = %d\n", block->a, block->b);
    printf("\n");

    printf("All adresses [0]: %p   %p   %p   %p\n", &block[0], &block[0].a, &block[0].b, &block[0].pntr);
    printf("All adresses [1]: %p   %p   %p   %p\n", &block[1], &block[1].a, &block[1].b, &block[1].pntr);
    printf("\n\n");


    int *a;
    // *a = 5; DOES NOT WORK!
    // printf("Number: %d\n", *a);

/* DOES NOT WORK!
    VAR *solo;
    solo->a = 80;
    solo->b = 160;
    printf("Zahl a = %d, Zahl b = %d\n", solo->a, solo->b);
*/

    return 0;
}