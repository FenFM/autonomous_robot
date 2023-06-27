#include <stdio.h>


typedef struct inner{
    int a;
    int b;
} INNER;


typedef struct outer{
    INNER x[2];
    INNER y;
} OUTER;


void pm_one(OUTER *var){
    var->x[0].a ++;
    var->x[1].a ++;
    var->x[0].b ++;
    var->x[1].b ++;
    var->y.a --;
    var->y.b --;
}


int main(int argc, char**argv){

    OUTER variable;
    variable.x[0].a = 1;
    variable.x[1].a = 10;
    variable.x[0].b = 2;
    variable.x[1].b = 20;
    variable.y.a = 3;
    variable.y.b = 4;

    printf("\n");
    printf("All Values: %2d  %2d  %2d  %2d  %2d  %2d\n", variable.x[0].a, variable.x[1].a, variable.x[0].b, variable.x[1].b, variable.y.a, variable.y.b);

    pm_one(&variable);
    printf("All Values: %2d  %2d  %2d  %2d  %2d  %2d\n", variable.x[0].a, variable.x[1].a, variable.x[0].b, variable.x[1].b, variable.y.a, variable.y.b);
    printf("\n");

    INNER one, two;
    one.a = 10;
    one.b = 20;

    two = one;

    printf("One: %2d   %2d   %p\n", one.a, one.b, &one);
    printf("Two: %2d   %2d   %p\n", two.a, two.b, &two);
    printf("\n");


    return 0;
}