#include "def.h"

FEDGE **edges;
int I, J /* , maxC */;
int **w, **t;
double *f_i, t_a, t_e, t_t, r_ij, alpha, C; //program parameters

// local helper methods
bool swap(int *h, int p, double *profit);

// main logic
double local_search(int **graph, int *z, int len, int *h, int p, double initial_profit)
{
    bool flag = true;
    double profit;
    int **vert;

    vert = create_and_initialize_int_matrix(len, len, -1);

    profit = initial_profit;
    restart_edge_matrix(edges, len, t, w);
    
    while (flag)
        flag = shift2(graph, len, h, p, z, &profit, vert);
    //flag = shift(graph, len, h, p, z, &profit, edges);

    return profit;
}

bool shift(int **graph, int len, int *h, int p, int *z, double *profit, FEDGE **edges)
{
    // select a vertex to shift it into the h and run all the process to get the new profit
    // the selected vertex will be the neighbor with the lowest ratio
    int *tmp_h, *tmp_z, *best_h, *best_z;
    int **sp = create_int_matrix(len, len);
    FEDGE **tmp_edges, **best_edges;

    tmp_h = create_int_vector(p);
    tmp_z = create_int_vector(len);
    best_h = create_int_vector(p);
    best_z = create_int_vector(len);

    // boolean matrix to hold the already shifted pair of nodes
    int **shifted = create_and_initialize_int_matrix(len, len, 0);

    tmp_edges = initialize_edge_matrix(t, w, len);
    best_edges = initialize_edge_matrix(t, w, len);

    bool res = false;
    for (int i = 0; i < p; i++)
    {
        int node_out = h[i];
        clone_int_vector(h, tmp_h, p);
        clone_int_vector(z, tmp_z, len);
        // traverse all nodes to check
        for (int j = 0; j < len; j++)
        {
            if (j != node_out && !isHub(tmp_z, j) && shifted[node_out][j] == 0 && shifted[j][node_out] == 0 )
            {
                //printf("shifting node %d and %d",node_out, j);
                clone_edge_matrix(edges, tmp_edges, len);

                shifted[node_out][j] = 1;
                shifted[j][node_out] = 1;

                for (int k = 0; k < len; k++)
                {
                    if(k == node_out)
                        tmp_z[k] = 0;
                    else if (k == j)
                        tmp_z[k] = 1;
                }
                int h_len = 0;
                for (int l = 0; l < len; l++)
                {
                    if (tmp_z[l] == 1)
                    {
                        tmp_h[h_len] = l;
                        h_len++;
                    }
                }
                sp = floydWarshallMod(t, tmp_z, len, p, tmp_h, tmp_edges);
                double new_profit = calculate_total_profit(tmp_edges, I, w, tmp_h, p, f_i, alpha, t_a, t_e, C);

                if (new_profit > *profit)
                {
                    *profit = new_profit;
                    clone_int_vector(tmp_h, best_h, p);
                    clone_int_vector(tmp_z, best_z, len);
                    clone_edge_matrix(tmp_edges, best_edges, len);
                    res = true;
                    break;
                    //printf("...improved profit found: %5.2f\n", new_profit);
                    //printVector(best_h, p, "best h: ");
                    //print_edges_analysis(best_edges, I);
                }
                //printf("\n");
                node_out = j;
            }
        }
    }
    if(res){
        clone_int_vector(best_h, h, p);
        clone_int_vector(best_z, z, len);
        clone_edge_matrix(best_edges, edges, len);
    }

    free(tmp_z);
    free(tmp_h);
    free(best_h);
    free(best_z);
    destroy_edges(tmp_edges, len);
    destroy_edges(best_edges, len);
    free_int_matrix(sp, len);

    return res;
}

bool shift2(int **graph, int len, int *h, int p, int *z, double *profit, int **vert)
{
    // select a vertex to shift it into the h and run all the process to get the new profit
    // the selected vertex will be the neighbor with the lowest ratio
    int *tmp_h, *tmp_z, *best_h, *best_z, **best_vert;
    int **sp = create_int_matrix(len, len);

    tmp_h = create_int_vector(p);
    tmp_z = create_int_vector(len);
    best_h = create_int_vector(p);
    best_z = create_int_vector(len);
    best_vert = create_int_matrix(len, len);

    // boolean matrix to hold the already shifted pair of nodes
    int **shifted = create_and_initialize_int_matrix(len, len, 0);

    bool res = false;
    for (int i = 0; i < p; i++)
    {
        int node_out = h[i];
        clone_int_vector(h, tmp_h, p);
        clone_int_vector(z, tmp_z, len);
        set_int_matrix(vert, len, len, 0); 

        // traverse all nodes to check
        for (int j = 0; j < len; j++)
        {
            if (j != node_out && !isHub(tmp_z, j) && shifted[node_out][j] == 0 && shifted[j][node_out] == 0 )
            {
                //printf("shifting node %d and %d",node_out, j);

                shifted[node_out][j] = 1;
                shifted[j][node_out] = 1;

                for (int k = 0; k < len; k++)
                {
                    if(k == node_out)
                        tmp_z[k] = 0;
                    else if (k == j)
                        tmp_z[k] = 1;
                }
                int h_len = 0;
                for (int l = 0; l < len; l++)
                {
                    if (tmp_z[l] == 1)
                    {
                        tmp_h[h_len] = l;
                        h_len++;
                    }
                }
                floydWarshallMod2(t, tmp_z, len, p, tmp_h, sp, vert);
                //double new_profit = calculate_total_profit(tmp_edges, I, w, tmp_h, p, f_i, alpha, t_a, t_e, C);
                // printMatrix(sp, len, len, "sp: ");
                // printMatrix(vert, len, len, "vert: ");
                // printVector(tmp_z, len, "tmp_z: ");
                double new_profit = calculate_total_profit2(tmp_z, vert, sp, len, t, w, f_i, alpha, t_a, t_e, C);

                if (new_profit > *profit)
                {
                    *profit = new_profit;
                    clone_int_vector(tmp_h, best_h, p);
                    clone_int_vector(tmp_z, best_z, len);
                    clone_int_matrix(vert, best_vert, len, len);
                    res = true;
                    break;
                    //printf("...improved profit found: %5.2f\n", new_profit);
                    //printVector(best_h, p, "best h: ");
                    //print_edges_analysis(best_edges, I);
                }
                //printf("\n");
                node_out = j;
            }
        }
    }
    if(res){
        clone_int_vector(best_h, h, p);
        clone_int_vector(best_z, z, len);
        clone_int_matrix(best_vert, vert, len, len);
        free_int_matrix(best_vert, len);
    }

    free(tmp_z);
    free(tmp_h);
    free(best_h);
    free(best_z);
    free_int_matrix(sp, len);
    free_int_matrix(shifted, len);

    return res;
}

bool swap(int *h, int p, double *profit)
{
    // for this is hard to c, but the hub edges would change and then run all the profit process
    bool res = true;
    return res;
}
