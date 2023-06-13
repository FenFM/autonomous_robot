
/*
*
*	tools.h
*
*	jsut some simple tools.
*
*/

extern int ferr( char *txt, char *cmt, int para );
extern char *m_alloc( int size, char *name );
extern char *m_free( char *p, char *name );
extern int prt_dot( int run );
extern char *tst_opt( char *opt, char *name );
extern int i_arg( char *opt, int *var, char *name );
extern int f_arg( char *opt, double *var, char *name );
extern char *mk_name( char *n1, char *n2 );
