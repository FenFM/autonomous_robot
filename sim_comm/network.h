#define PI 3.141
#define SIG_START 1
#define N_INPUT_LAYER  6
#define N_HIDDEN_LAYER_1 8                  // must be devidable by 4
#define N_HIDDEN_LAYER_2 N_HIDDEN_LAYER_1
#define N_OUTPUT_LAYER 2
#define N_OF_DATA_SETS 2
#define N_OF_CHILDS       50
#define N_OF_GENERATIONS 200


typedef struct unit{
    double netinput;
    double output;
    double activation;
    double weight;
    double sigma;
} UNIT;


typedef struct network{
    UNIT input_layer   [N_INPUT_LAYER] ;
    UNIT hidden_layer_1[N_HIDDEN_LAYER_1];
    UNIT hidden_layer_2[N_HIDDEN_LAYER_2];
    UNIT output_layer  [N_OUTPUT_LAYER];
    int n_input;
    int n_hidden_1;
    int n_hidden_2;
    int n_output;
} NETWORK;


typedef struct memspace{
    short int comm_flag;
    short int IR_Distance[6];
    short int Motor_Value[2];
} MemSpace;


double my_square(double input);
double gauss();
double calc_output(UNIT unit);
void calc_network(NETWORK *network, double *in_data);
void mutate_network(NETWORK *network);
void set_start_values(NETWORK *network);
double calc_fitness(NETWORK network, double *out_data);
int get_best_fitness(double *fitness, int n_child);
double calc_one_generation(NETWORK *network, double *in_data, double *out_data, int n_data, int n_child);
