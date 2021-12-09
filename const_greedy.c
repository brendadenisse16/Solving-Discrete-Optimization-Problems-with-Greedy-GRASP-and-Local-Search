#include "def.h"

FEDGE *edge_list;
//FEDGE **edges;
FNODE *node_list;
double *f_i, *f_j, max_f_i, max_f_j;
double t_t;
int I, J /* , maxC */;
int **w, **t;
int *z;                               //node index of hubs
int **y;                              //edge index of hub edges
double t_a, t_e, t_t, r_ij, alpha, C; //program parameters

// main logic
double const_greedy(int p, int *h, int **sp, int len, FEDGE **edges)
{
    int **vert;
    double total_profit;
    vert = create_int_matrix(len, len);
    set_int_matrix(vert, len, len, -1);
    //edges = initialize_edge_matrix(t, w, I);
    node_list = create_node_list(len);
    populate_node_list(node_list, len, t, w);
    sort_nodes(node_list, len, ASC);
    for (int i = 0; i < p; i++)
        z[node_list[i].n] = 1;

    clone_int_vector(generate_edge_vector(z, I, p), h, len);
    //floydWarshallMod(t, z, I, p, h, edges);
    //double total_profit = calculate_total_profit(edges, I, w, h, p, f_i, alpha, t_a, t_e, C);
    floydWarshallMod2(t, z, I, p, h, sp, vert);
    total_profit = calculate_total_profit2(z, vert, sp, I, t, w, f_i, alpha, t_a, t_e, C);
    return total_profit;
}