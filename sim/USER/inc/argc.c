
#include <stdio.h>
#include "default.h"
#include "tools.h"
#include "argc.h"

int std_opts( int argc, char **argv, int *mu, int *lambda, int *plus, int *prob_size,
	      int *generations, int *modulo, int *prt_opts, double *sigma, double *x0 )
    {
	char *p, *arg, *f_name = 0;
	int ac = 0;
	while((p = next_arg( argc, argv )))
	{
	   if ( ! strcmp( p, "-h" ) || ! strcmp( p, "?" ) )
	   {
              fprintf( stderr, "Standard options are Possible options: " );
              fprintf( stderr, "mu lambda plus prob_size generations modulo prt_opts " );
              fprintf( stderr, "sigma x0 \n\n" );
	      exit( 2 );
	   }
	   if ( 
	           f_arg( p, sigma,       "sigma"       )
	        || i_arg( p, prt_opts,    "prt_opts"    )
	        || i_arg( p, generations, "generations" )
	        || i_arg( p, prob_size,   "prob_size"   )
	        || i_arg( p, mu,          "mu"          )
	        || i_arg( p, lambda,      "lambda"      )
	        || i_arg( p, plus,        "plus"        )
	        || f_arg( p, x0,          "x0"          )
	        || i_arg( p, modulo,      "modulo"      )
	      )
	      ;
	   else if ( is_cmdline() )
		   argv[ ac++ ] = p;
	   else {
	         fprintf( stderr, "%s: unknown option '%s' ", *argv, p );
	         fprintf( stderr, "in %s\n", cur_file() );
	         exit( 1 );
		}
	}
	return ac;
    }
