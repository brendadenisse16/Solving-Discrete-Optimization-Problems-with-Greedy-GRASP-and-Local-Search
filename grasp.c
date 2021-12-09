#include "def.h"
int **w, **t;
double *f_i,t_a, t_e,C;

double grasp(int max_iter_grasp, double alpha, int len, int p, double impr_prc, double orig_profit, FEDGE **edges)
{
    int *best_h, *best_z;
    int increase = 0;
    int *z, *h, **sp;
    FEDGE **best_edges;
    double grasp_profit = 0.0;
    
    best_h = create_int_vector(p);
    best_z = create_int_vector(len);
    best_edges = initialize_edge_matrix(t, w, len);

    z = create_and_initialize_int_vector(len, 0);
    h = create_and_initialize_int_vector(p, -1);

    sp = create_and_initialize_int_matrix(len, len, 0);

    FNODE *node_list = create_node_list(len);
    // here we add the node ratio values and populate the FNODE list
    populate_node_list(node_list, len, t, w);
    sort_nodes(node_list, len, ASC);
    int min_ratio = get_min_node_ratio(node_list, len);
    int max_ratio = get_max_node_ratio(node_list, len);
    //FEDGE **edges = initialize_edge_matrix(t, w, len);
    

    unsigned seed = ((unsigned)time(NULL));
    srand(seed);

    for (int iter = 0; iter < max_iter_grasp; iter++)
    {
        int window = 0;
        int feasible = 1;
        set_int_vector(z, len, 0);
        set_int_vector(h, p, -1);
        restart_edge_matrix(edges, len, t, w);

        // get the max and min ratio values
        // if inode.ratio < min_ratio + alpha * (max_ratio - min_ratio) -> flag++
        int count = 0; //counts the nodes to analyze
        for (int i = 0; i < len; i++)
        {
            if (node_list[i].ratio <= min_ratio + alpha * (max_ratio - min_ratio))
                window++;
            else
                break;
        }
        //printf("found %d hub candidates... \n", window);
        // get #p random elements from the list
        for (int i = 0; i < p; i++)
        {
            int found = 0;
            int failover_count = 0;
            do
            {
                int candidate_index = getrandom(0, window - 1);
                int node = node_list[candidate_index].n;
                if(found < p && z[node] == 0)
                {
                    h[i] = node;
                    z[node] = 1;
                    found++;
                    break;
                }
                failover_count++;
            } while (found <= p && failover_count <= window );
            if ( failover_count > window )
            {
                //printf("failover count reached!\n");
                feasible = 0;
                break;
            }
        }
        if (feasible == 1)
        {
            floydWarshallMod(t, z, len, p, h, edges);
            double profit = calculate_total_profit(edges, len, w, h, p, f_i, alpha, t_a, t_e, C);
            
            bool flag = true;
            while (flag)
            {
                flag = shift(t, len, h, p, z, &profit, edges);
            }

            if (profit > grasp_profit)
            {
                grasp_profit = profit;
                //printf("Iteration %d profit: %5.5f\n", iter, grasp_profit);
                clone_int_vector(h, best_h, p);
                clone_int_vector(z, best_z, len);
                clone_edge_matrix(edges, best_edges, len);

                //printVector(h, p, "hubs:: ");
            }
            if ( grasp_profit > orig_profit && 
                    (grasp_profit-orig_profit)/orig_profit > impr_prc )
            {
                //printf("Grasp percentage improve target met!!!\n");
                break;
            }
        }
        if (increase == 500)
        {
            alpha += 0.03; //slighly increase the alpha margin
            increase = 0;
        } else
        {
            increase++;
        }
        if (alpha >= 1)
            alpha = 0.01;
    }
    // clone_int_vector(best_h, h, p);
    // clone_int_vector(best_z, z, len);
    clone_edge_matrix(best_edges, edges, len);

    free(z);
    free(h);
    for (int i = 0; i < len; i++)
        free(sp[i]);

    free(best_h);
    free(best_z);
    destroy_edges(best_edges, len);

    return grasp_profit;
}