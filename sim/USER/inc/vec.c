
#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "vec.h"
#include "rand.h"

VP vec_new( int size )
   {
      VP p;
      p = (VP) m_alloc( sizeof( VEC ), "vec_new1" );
      p->size = size;
      p->pts = (double *) m_alloc( sizeof( double ) * size, "vec_new2" );
      return( p );
   }

int vec_free( VP p )
    {
       m_free( (char *) p->pts, "vec_free1" );
       m_free( (char *) p, "vec_free2" );
    }

int vec_init( VP p, double val, double sigma )
    {
       int i;
       for( i = 0; i < p->size; i++ )
	  p->pts[ i ] = val;
       p->sigma = sigma;
    }

int vec_rnd( VP p )
    {
       int i;
       for( i = 0; i < p->size; i++ )
	  p->pts[ i ] = gauss();
    }

int vec_add_gauss( VP p )
    {
       int i;
       for( i = 0; i < p->size; i++ )
	  p->pts[ i ] += p->sigma * gauss();
    }

double vec_len( VP p )
       {
	  int i;
	  double len;
	  for( i = 0, len = 0; i < p->size; i++ )
	     len += p->pts[ i ] * p->pts[ i ];
	  return( sqrt( len ) );
       }

int vec_scale( VP p, double scale )
    {
       int i;
       for( i = 0; i < p->size; i++ )
	  p->pts[ i ] *= scale;
    }

int vec_norm( VP p )
    {
       vec_scale( p, 1.0 / vec_len( p ) );
    }

int vec_prt( FILE *fp, VP p, int opt )
    {
       int i;
       if ( opt & 0x1 )
	  fprintf( fp, "size=%d  ", p->size );
       if ( opt & 0x2 )
	  fprintf( fp, "len=%e  ", vec_len( p ) );
       for( i = 0; i < p->size; i++ )
	  fprintf( fp, "%e  ", p->pts[ i ] );
    }

int vec_cp( VP from, VP to )
    {
       int i;
       to->f = from->f;
       to->sigma = from->sigma;
       for( i = 0; i < to->size; i++ )
	  to->pts[ i ] = from->pts[ i ];
    }

int vec_add( VP dest, VP op1, VP op2 )
    {
       int i;
       for( i = 0; i < dest->size; i++ )
	  dest->pts[ i ] = op1->pts[ i ] + op2->pts[ i ];
    }

int vec_inc( VP dest, VP inc )
    {
       int i;
       for( i = 0; i < dest->size; i++ )
	  dest->pts[ i ] += inc->pts[ i ];
    }
