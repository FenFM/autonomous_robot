
#include <stdio.h>
#include <stdlib.h>
#include "ea.h"
#include "vec.h"
#include "tools.h"
#include "rand.h"

typedef struct {
		 int problem_size;	/* number of object parameters x_i */
		 int mu;
		 int lambda;
		 int size;		/* size = mu + lambda */
		 VP *table;
	       } TABLE, *T_P;

#define		P_INDEX( i )	(i)
#define		O_INDEX( i )	(tp->mu + i)

static T_P tp = 0;

static int progerr( char *fnc, char *txt, int arg, int quit )
       {
          fflush( stdout );
          fprintf( stderr,"%s: sorry, but you have made a fatal programming error\n",fnc);
	  if ( txt )
	  {
             fprintf( stderr, "\t" );
             fprintf( stderr, txt, arg );
             fprintf( stderr, "\n" );
	  }
	  if ( quit )
	     exit( 1 );
       }

#define fprogerr( fnc, txt, arg )	progerr( fnc, txt, arg, 1 )	/* fatal error */

static int check_table( char *fnc )
       {
	  if ( ! tp )
	     fprogerr( fnc, "no populations present :-(", 0 );
       }

static int check_poptype( char *fnc, int i )
       {
	  if ( i != PARENT && i != OFFSPRING )
	     fprogerr( fnc, "no such population type %d", i );
       }

static VP check_parent( char *fnc, int p )
       {
	  check_table( fnc );
	  if ( p < 0 || p >= tp->mu )
	     fprogerr( fnc, "no such parent %d", p );
	  return tp->table[ P_INDEX( p ) ];
       }

static VP check_offspring( char *fnc, int o )
       {
	  check_table( fnc );
	  if ( o < 0 || o >= tp->lambda )
	     fprogerr( fnc, "no such offspring %d", o );
	  return tp->table[ O_INDEX( o ) ];
       }

static VP check_object( char *fnc, int pop_type, int o )
       {
	  check_table( fnc );
	  check_poptype( fnc, pop_type );
	  return (pop_type == PARENT)? check_parent( fnc, o ): check_offspring( fnc, o );
       }

static int check_parameter( char *fnc, int i )
       {
	  check_table( fnc );
	  if ( i < 0 || i >= tp->problem_size )
	     fprogerr( fnc, "no such parameter (index) %d", i );
       }

int make_populations( int mu, int lambda, int problem_size )
    {
       int i;
       if ( tp )
          fprogerr( "make_populations", "you have already created the populations; destroy them first", 0 );
       if ( mu < 1 )
          fprogerr( "make_populations", "you need at least mu=1 parent (rather than %d)", mu );
       if ( lambda < 1 )
          fprogerr( "make_populations", "you need at least lambda=1 offspring (rather than %d)", lambda );
       if ( problem_size < 1 )
          fprogerr( "make_populations", "you need at least one free parameter x_i (rather than %d)", problem_size );
       tp = (T_P) m_alloc( sizeof(TABLE), "make_populations1" );
       tp->problem_size = problem_size;
       tp->mu = mu;
       tp->lambda = lambda;
       tp->size   = mu + lambda;
       tp->table  = (VP *) m_alloc( sizeof(VP) * tp->size, "make_populations2" );
       for( i = 0; i < tp->size; i++ )
       {
          tp->table[ i ] = vec_new( problem_size );
	  vec_init( tp->table[ i ], 0.0, 0.0 );
       }
       return 1;
    }

int destroy_populations()
    {
       int i;
       check_table( "destroy_populations" );
       for( i = 0; i < tp->size; i++ )
	  vec_free( tp->table[ i ] );
       m_free( (char *) tp->table, "destroy_populations1" );
       m_free( (char *) tp, "destroy_populations2" );
       tp = 0;
    }

int init_parents( double min_x, double max_x, double sigma )
    {
       int i, j;
       double dif = max_x - min_x;
       check_table( "init_parents" );
       for( i = 0; i < tp->mu; i++ )
       {
	  tp->table[ P_INDEX( i ) ]->sigma = sigma;
          for( j = 0; j < tp->problem_size; j++ )
	     tp->table[ P_INDEX( i ) ]->pts[ j ] = min_x + dif * drand();
       }
    }

double get_parameter( int pop_type, int o, int i )
       {
          check_parameter( "get_parameter", i );
	  check_poptype( "get_parameter", pop_type );
	  return ((pop_type == PARENT)? check_parent( "get_parameter", o )
	                              : check_offspring( "get_parameter", o ))->pts[ i ];
       }

int set_parameter( int pop_type, int o, int i, double val )
    {
       check_parameter( "set_parameter", i );
       check_poptype( "set_parameter", pop_type );
       ((pop_type == PARENT)? check_parent( "get_parameter", o )
	                    : check_offspring( "get_parameter", o ))->pts[ i ] = val;
    }

double get_sigma( int pop_type, int o )
       {
          check_poptype( "set_sigma", pop_type );
	  return ((pop_type == PARENT)? check_parent( "get_sigma", o )
	                              : check_offspring( "get_sigma", o ))->sigma;
       }

int set_sigma( int pop_type, int o, double sigma )
    {
       check_poptype( "set_sigma", pop_type );
       ((pop_type == PARENT)? check_parent( "set_sigma", o )
			    : check_offspring( "set_sigma", o ))->sigma = sigma;
    }

double get_fitness( int pop_type, int o )
       {
          check_poptype( "set_fitness", pop_type );
	  return ((pop_type == PARENT)? check_parent( "get_fitness", o )
	                             : check_offspring( "get_fitness", o ))->f;
       }

int set_fitness( int pop_type, int o, double fitness )
    {
       check_poptype( "set_fitness", pop_type );
       ((pop_type == PARENT)? check_parent( "set_fitness", o )
			    : check_offspring( "set_fitness", o ))->f = fitness;
    }

double get_faverage()
       {
          double f;
	  int i;
	  for( i = 0, f = 0; i < tp->mu; i++ )
	     f += tp->table[ P_INDEX( i ) ]->f;
	  return f / tp->mu;
       }

int copy_objects( int from_pop, int from_index, int to_pop, int to_index )
    {
       vec_cp( check_object( "copy_objects", from_pop, from_index ),
               check_object( "copy_objects", to_pop,   to_index ) );
    }

static int swap_obj( int i, int j )
       {
	  VP p;
	  p = tp->table[ i ];
	  tp->table[ i ] = tp->table[ j ];
	  tp->table[ j ] = p;
       }

static int bubble( int start, int end )
       {
	  int i, j, m;
	  for( i = start; i < end - 1; i++ )
	  {
	     for( m = i, j = i + 1; j < end; j++ )
		if ( tp->table[ j ]->f > tp->table[ m ]->f )
		   m = j;
	     if ( m != i )
	        swap_obj( i, m );
	  }
       }

int selection( int smode )
    {
       int i;
       check_table( "selection" );
       if ( smode == COMMA && tp->lambda < tp->mu )
          fprogerr( "selection", "hey, this does not make any sense, lambda must\n\tbe greater than mu when using the ',' strategy", 0 );
       switch( smode )
       {
          case PLUS:
	  	      bubble( 0, tp->size );
	  	      break;
	  case COMMA:
	  	      bubble( O_INDEX( 0 ), O_INDEX( 0 ) + tp->lambda );
       		      for( i = 0; i < tp->mu; i++ )
	  		 swap_obj( P_INDEX( i ), O_INDEX( i ) );
	  	      break;
	  default:    fprogerr( "selection", "no such selection mode %d", smode );
       }
    }

int mutate_offspring( int i )
    {
       VP p = check_offspring( "mutate_offspring", i );
       switch( n_choice( 3 ) )
       {
	  case 0: p->sigma *= 1.5; break;
	  case 1: p->sigma /= 1.5; break;
       }
       vec_add_gauss( p );
    }
