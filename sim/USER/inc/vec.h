
typedef struct {
		 int size;
		 double sigma;
		 double f;	/* fitness */
		 double *pts;
	       } VEC, *VP;

extern VP vec_new( int size );
extern int vec_free( VP p );
extern int vec_init( VP p, double val, double sigma );
extern int vec_rnd( VP p );
extern int vec_add_gauss( VP p );
extern double vec_len( VP p );
extern int vec_scale( VP p, double scale );
extern int vec_norm( VP p );
extern int vec_prt( FILE *fp, VP p, int opt );
extern int vec_inc( VP dest, VP inc );
extern int vec_cp( VP from, VP to );
extern int vec_add( VP dest, VP op1, VP op2 );
