#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <float.h>
#include <stdbool.h>
//#include "cplex.h"
//#include <igraph.h>

#define getrandom(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))
#define true 1
#define false 0
#define NOT_AVAIL -1
#define NONE -1
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define ASC 1
#define DESC 0
#define INF 9999
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
typedef struct FEDGE
{
	int i;
	int j;
	int t; // original direct time
	int w; // associated demand
	double sp; // obtained shortest path
	bool is_hub_edge; // also known as "x"
	bool is_direct; // also known as "e"
	bool is_access_edge; // also knonw as "a"
	bool is_exit_edge; // also knonw as "b"
	bool has_saving; // also knonw as "tetha"
	double ratio;
} FEDGE;
typedef struct FNODE
{
	int n;
	bool is_hub;
	double ratio;
} FNODE;
typedef struct STACK
{
	int *list;
	int top;
} STACK;
// FROM  ReadData
void read_data(const char *name);
void initialize_local_memory(void);
void free_local_memory(void);
void calculate_internals(void);
void read_costs_file(const char *name, int len);

// from const_greedy
double const_greedy(int p, int *h, int **sp, int len, FEDGE **edges);

// from graph
FEDGE **initialize_edge_matrix(int **t, int **w, int len);
int *generate_edge_vector(int *z, int len, int p);
void print_edges_analysis(FEDGE **edges, int len);
void clone_edge_matrix(FEDGE **origin, FEDGE **target, int len);
void destroy_edges(FEDGE **edges, int len);
void restart_edge_matrix(FEDGE **edges, int len, int **t, int **w);
void populate_node_list(FNODE *node_list, int len, int **t, int **w);
void sort_nodes(FNODE *node_list, int len, int order);
int compare_nodes_asc(const void *a, const void *b);
int compare_nodes_desc(const void *a, const void *b);
FNODE *create_node_list(int len);
double get_max_node_ratio(FNODE *node_list, int len);
double get_min_node_ratio(FNODE *node_list, int len);

// from floydWarshall
int **floydWarshallMod(int **graph, int *z, int n_nodes, int p, int *h, FEDGE **edges);
void floydWarshallMod2(int **graph, int *z, int n_nodes, int p, int *h, int **dist, int **vert);
bool isHub(int *z, int k);
// from local_search
double local_search(int **graph, int *z, int len, int *h, int p, double initial_profit);
bool shift(int **graph, int len, int *h, int p, int *z, double *profit, FEDGE **edges);
bool shift2(int **graph, int len, int *h, int p, int *z, double *profit, int **vert);
// from grasp
double grasp(int max_iter_grasp, double alpha, int len, int p, double impr_prc, double orig_profit, FEDGE **edges);
// from profit
double calculate_total_profit(FEDGE **edges, int len, int **w, int *h, int p, double *f_i, double alpha, double t_a, double t_e, double C);
double calculate_total_profit2(int *z, int **vert, int **sp, int len, int **t, int **w, double *f_i, double alpha, double t_a, double t_e, double C);
// from utils
double evaluate_solution(int *);
double evaluate_swapped_solution(int *, int, int);
void free_memory(void);
void Initialize_memory(void);
void i_vector(int **vector, int n, char *s);
void d_vector(double **vector, int n, char *s);
void c_vector(char **, int, char *);
int **create_int_matrix(int, int);
int **create_and_initialize_int_matrix(int, int, int);
double **create_double_matrix(int, int);
double **create_and_initialize_double_matrix(int, int, double);
int *create_int_vector(int);
int *create_and_initialize_int_vector(int, int);
double *create_double_vector(int);
double *create_and_initialize_double_vector(int, double);
void set_int_vector(int *vector, int len, int value);
void set_int_matrix(int **vector, int m, int n, int value);
int Swap(int *, int *, double *);
int Shift(int *, int *, double *);
int Swap_opt(int *, int *, int *, double *);
void printMatrix(int **matrix, int a, int b, char *label);
void printVector(int *vector, int l, char *label);
int *copy_int_vector(int *vector, int n);
int **copy_int_matrix(int **matrix, int i, int j);
STACK create_stack(int listLen);
void clean_stack(STACK *stack);
void push(int elem, STACK *stack);
int pop(STACK *stack);
void free_int_matrix(int **matrix, int rows);
void printDoubleMatrix(double **matrix, int a, int b, char *label);
double *copy_double_vector(double *vector, int n);
double **copy_double_matrix(double **matrix, int i, int j);
void shift_in_list(int *list, int len, int pos, int elem);
double elapsed_time(clock_t start, clock_t end);
void clone_int_vector(int *origin, int *target, int len);
void clone_int_matrix(int **origin, int **target, int n, int m);