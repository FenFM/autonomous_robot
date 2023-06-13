
#include <stdio.h>
#include <math.h>
#include "argc.h"
#include "ea.h"

/* global variables */

int generations = XX, prt_opts = 0;
int mu = XX, lambda = XX, prob_size = XX;
double sigma = XX;
int plus = XX;
double x0 = XX;
int modulo = XX;

main( argc, argv )
int argc;
char **argv;
{
	int i, g;
	argc = std_opts( argc, argv, & mu, & lambda, & plus, & prob_size,
			 & generations, & modulo, & prt_opts, & sigma, & x0 );
	if ( argc > 0 )
	{
	   // room to process own options
	   for( i = 0; i < argc; i++ )
	      fprintf( stderr, "warning, unprocessed option: '%s'\n", argv[ i ] );
	   fprintf( stderr, "\n" );
	}

        ...
}
