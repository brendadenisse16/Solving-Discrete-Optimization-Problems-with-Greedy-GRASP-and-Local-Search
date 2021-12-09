#include "def.h"

int I;    //number of nodes to be selected
int **sp; //shortest path found

int **t, **w, **y, *z;
extern double C, alpha, betha, t_a, t_e, t_t, v, r_ij; //program general parameters
int *h, p;
FEDGE **edges;
double *f_i;

// local functions declaration
void firstGraph(void);
void secondGraph(void);
void thirdGraph(void);
int main(void)
{
    clock_t start, end;
    // program general parameters
    p = 3;       // how many hubs nodes are needed
    C = 1.25;    // cost penalty for the direct
    alpha = 0.8; // discount factor for using the hub
    v = 0.0;     // for the transfer time
    // grasp parameters
    int grasp_iter = 100;
    double grasp_alpha = 0.03;
    double grasp_stop_percent = 0.03;

    read_data("data5.tsv");
    read_costs_file("cab_data_setup_cost.txt", I);
    calculate_internals();
     printMatrix(w, I, I, "Demand matrix: ");
     printMatrix(t, I, I, "Time matrix: ");

    h = create_int_vector(p);
    edges = initialize_edge_matrix(t, w, I);
    
    initialize_local_memory();
    start = clock();
    double greedy_profit = const_greedy(p, h, sp, I, edges);
    end = clock();
    // print_edges_analysis(edges, I);
    printf("\nGreedy profit: %5.5f, elapsed time %5.6f\n", greedy_profit, elapsed_time(start,end));

    start = clock();
    double local_search_profit = local_search(t, z, I, h, p, greedy_profit);
    end = clock();
    
    printf("\nL. S. profit: %5.5f, elapsed time %5.6f, improvement %5.2f \n", local_search_profit, elapsed_time(start,end), (local_search_profit - greedy_profit)/greedy_profit);
    // print_edges_analysis(edges, I);

    // start = clock();
    // double grasp_profit = grasp(grasp_iter, grasp_alpha, I, p, grasp_stop_percent, local_search_profit, edges);
    // end = clock();
    
    // printf("\nGrasp profit: %5.5f, elapsed time %5.6f, improvement %5.2f \n", grasp_profit, elapsed_time(start,end), (grasp_profit - local_search_profit)/local_search_profit);
    // print_edges_analysis(edges, I);
    //analize the parameters for the histogram:
    // * 

    free_local_memory();

    // firstGraph();
    // secondGraph();
    // thirdGraph();
    return EXIT_SUCCESS;
}