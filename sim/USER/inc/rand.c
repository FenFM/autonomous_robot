
/* random number generators */

#include <math.h>
#include <stdio.h>
#include "rand.h"

#define TSIZE   (sizeof(table)/sizeof(char *))

#define genx(A) ((3141 * A + 87591) & 0xFFFF)
#define VSIZE 128

typedef struct {
		 long rd_v[ VSIZE ];
		 long rd_x;
		 long rd_y;
		 int  rd_j;
	       } RAND;

static RAND rand;
static int init = 0;

/* some gauss tables */

static double a[] = { 0.0, 0.0, 0.0, 0.25, 0.25, 0.5, 0.5, 0.75, 1.0, 1.25 },
              b[] = { 0.25, 0.25, 0.25, 0.5, 0.75, 0.75, 0.75, 1.0, 1.5, 1.5,
                      1.75, 2.0 },
              c[] = { 0.0, 0.25, 0.5, 0.5, 0.75, 0.75, 1.0, 1.0, 1.0, 1.5, 1.5,
                      1.5, 1.75, 1.75, 2.25, 2.25, 2.5 },
              s[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25,
                      2.5, 2.75, 3.0 },
              p[] = { 0.885, 0.895, 0.910, 0.929, 0.945, 0.960, 0.971, 0.982,
                      0.987, 0.991, 0.994, 0.997, 1.0 },
              q[] = { 0.881, 0.885, 0.897, 0.914, 0.930, 0.947, 0.960, 0.974,
                      0.982, 0.989, 0.992, 0.996 },
              d[] = { 0.51, 0.79, 0.90, 0.98, 0.99, 0.99, 0.98, 0.96, 0.95,
                      0.93, 0.94, 0.94 },
              e[] = { 16.0, 8.0, 5.33, 4.0, 3.08, 2.44, 2.0, 1.67, 1.43, 1.23,
                      1.08, 0.95 };


static int rndinit()
       {
	  register int i;
	  init = 1;
          rand.rd_v[ 0 ] = 152;
          for( i = 1; i < VSIZE; i++ )
             rand.rd_v[i] = genx( rand.rd_v[i-1] );
	  rand.rd_y = 73;
          rand.rd_x = rand.rd_v[VSIZE - 1];
	  rand.rd_j = VSIZE - 1;
       }

long uRand()
     {
	if ( ! init )
	   rndinit();
        rand.rd_v[ rand.rd_j ] = rand.rd_x;
        rand.rd_x = genx( rand.rd_x );
        rand.rd_y = (7593 * rand.rd_y + 43141) & 0xFFFF;
        rand.rd_j = VSIZE * rand.rd_y / 65536;
        return( rand.rd_v[ rand.rd_j ] );
     }

double drand()
       {  return( uRand() / 65536.0 );
       }

int choice()
    {  return( ! ( lrand48() & 0x1 ) );
    }

int n_choice( int n )
    {  return( (n > 1)? lrand48() % n: 0 );
    }


double xx_rand()
       {
	  double drand48();
	  double x = 1.0e-8*exp( 1.0 - log( 1.0e-8 ) * drand48() );
	  return( x );
       }

double gauss()
       {
          unsigned long z, z15, sign;
	  register int idx, j;
          double w, t, h, sjp2;
          register double x, zflt, u, v;
          sign = ((z = (*uRand)()) > 32767);
          z15 = z & 0x7FFF;
          if ((idx = z15 >> 11) < 10 )
                  x = a[idx] + (z & 0x7FF) / 8192.0;
          else if ((idx = z15 >> 9) < 52 )
	          x = b[idx-40] + (z & 0x1FF) / 2048.0;
          else if ((idx = z15 >> 7) < 225 )
	                  x = c[idx-208] + (z & 0x7F) / 512.0;
          else {
	         if ((zflt = z15 / 32786.0) < p[11] ) /* wedge */
	         {
	            for( j = 0; zflt > p[j]; j++ ) ;
	            if ( zflt < q[j] )
	               x = s[j] + ((*uRand)() / (4.0 * 65536.0));
	            else { 
		           sjp2 = s[j + 1] * s[j + 1];
		           do {
		                u = (*uRand)()/65536.0; v = (*uRand)()/65536.0;
		                if ( u > v )
		                {  h = u; u = v; v = h;
		                }
		                x = s[j] + u / 4.0;
		                u += e[j] * exp( (x*x - sjp2) / -2.0 ) - 1.0;
		              } while( v > d[j] || v > u );
		         }
	          }
	          else do {		 /* tail */
		            do {
		                 u = (*uRand)() / 65536.0;
				 v = (*uRand)() / 65536.0;
		                 w = u*u + v*v;
		               } while( w >= 1.0 );
		            t = sqrt( (9.0 - 2.0 * log( w )) / w );
		            x = (u * t >= 3)? u * t: v * t;
	                  } while( x < 3 );
               }
          return( (sign)? -x: x );
       }

double bga_mut()
       {
	  int i;
	  double val;
	  do for( i = 0, val = 0; i < 16; i++ )
		val = val / 2 + ! n_choice( 16 );
	  while( val == 0.0 );
	  return( (choice())? val: -val );
       }
