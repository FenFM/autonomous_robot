#define PARENT		0
#define OFFSPRING	1

#define COMMA		0
#define PLUS		1

extern int make_populations( int mu, int lambda, int problem_size );
extern int destroy_populations();
extern int init_parents( double min_x, double max_x, double sigma );
extern double get_parameter( int pop_type, int o, int i );
extern int set_parameter( int pop_type, int o, int i, double val );
extern double get_sigma( int pop_type, int o );
extern int set_sigma( int pop_type, int o, double sigma );
extern double get_fitness( int pop_type, int o );
extern int set_fitness( int pop_type, int o, double fitness );
extern double get_faverage();
extern int copy_objects( int from_pop, int from_index, int to_pop, int to_index );
extern int selection( int smode );
extern int mutate_offspring( int i );
