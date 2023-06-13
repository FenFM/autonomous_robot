
#include <stdio.h>
#include <ctype.h>

static int ac;
static char **av = 0;
static char buf[ 256 ];
static char *f_name;
static FILE *fp;

#define default ".defaults"

static char *readarg()
       {
	  int c, i = 0;
again:
	  while( isspace((c = getc( fp ))))
	     ;
	  if ( c == EOF )
	     return( (char *) 0 );
	  if ( c == '#' )
	  {
	     while((c = getc( fp )) != EOF && c != '\n' )
		;
	     goto again;
	  }
	  while( c != EOF && ! isspace( c ) && i < 255 )
	  {
	     buf[ i++ ] = c;
	     c = getc( fp );
	  }
	  buf[ i ] = '\0';
	  return( (i)? buf: (char *) 0 );
       }

char *next_arg( int argc, char **argv )
     {
	char *next;
	if ( ! av )	/* initialization */
	{
	   ac = argc - 1;
	   av = argv + 1;
	   fp = fopen( (f_name = default), "r" );
	}
	while( 1 )
	{
	   if ( fp )			/* processing open file */
	      if ((next = readarg()))
		 return( next );
	      else { 
		      fclose( fp ); fp = 0;
		      f_name = "command line";
		   } 
	   if ( ! ac )
	      return( 0 );
	   if ( (**av != '-' && **av != '.') || ! strcmp( *av, "-h" ) )
	   {
	      ac--;
	      return( *av++ );
	   }
	   fp = fopen( (f_name = *av + (**av == '-')), "r" ); ac--; av++;
	}
     }

int is_cmdline()
    {
       return fp == 0;
    }

char *cur_file()
     {
	return( f_name );
     }
