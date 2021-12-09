#include "def.h"
extern FEDGE **edges;
double *f_i, *f_j, max_f_i, max_f_j;

// local methods
int compare_edges_desc(const void *a, const void *b);
int compare_edges_asc(const void *a, const void *b);
double calculate_node_ratio(int i, int len, int **t, int **w);
void get_flow_usage(FEDGE **edges, int len);

FEDGE **initialize_edge_matrix(int **t, int **w, int len)
{
    // initialization
    FEDGE **edges = (FEDGE **)calloc(len, sizeof(FEDGE));
    if (edges == NULL)
    {
        printf("\nError: Insuficient memory \n");
        exit(8);
    }
    for (int i = 0; i < len; i++)
    {
        edges[i] = (FEDGE *)calloc(len, sizeof(FEDGE));
        if (edges[i] == NULL)
        {
            printf("\nError: Insuficient memory \n");
            exit(8);
        }
        for (int j = 0; j < len; j++)
        {
            edges[i][j].i = i;
            edges[i][j].j = j;
            edges[i][j].t = t[i][j];
            edges[i][j].sp = edges[i][j].t;
            edges[i][j].w = w[i][j];
            edges[i][j].is_access_edge = false;
            edges[i][j].is_exit_edge = false;
            edges[i][j].is_direct = false;
            edges[i][j].is_hub_edge = false;
            edges[i][j].has_saving = false;
        }
    }
    return edges;
}

void restart_edge_matrix(FEDGE **edges, int len, int **t, int **w)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            edges[i][j].i = i;
            edges[i][j].j = j;
            edges[i][j].t = t[i][j];
            edges[i][j].sp = edges[i][j].t;
            edges[i][j].w = w[i][j];
            edges[i][j].is_access_edge = false;
            edges[i][j].is_exit_edge = false;
            edges[i][j].is_direct = false;
            edges[i][j].is_hub_edge = false;
            edges[i][j].has_saving = false;
        }
    }
}

int *generate_edge_vector(int *z, int len, int p)
{
    int *h = create_int_vector(p);
    int h_len = 0;
    for (int i = 0; i < len; i++)
    {
        if (z[i] == 1)
            h[h_len++] = i;
    }
    return h;
}

void print_edges_analysis(FEDGE **edges, int len)
{
    // int **direct_edges = create_int_matrix(len, len);
    // int **access_edges = create_int_matrix(len, len);
    // int **hub_edges = create_int_matrix(len, len);
    // for (int i = 0; i < len; i++)
    // {
    //     for (int j = 0; j < len; j++)
    //     {
    //         direct_edges[i][j] = edges[i][j].is_direct ? 1 : 0;
    //         access_edges[i][j] = edges[i][j].is_access_edge ? 1 : 0;
    //         hub_edges[i][j] = edges[i][j].is_hub_edge ? 1 : 0;
    //     }
    // }
    // double direct_pct = 0.0;
    // double hub_pct = 0;
    // int access = 0;
    // int hubs = 0;
    // int directs = 0;

    get_flow_usage(edges, len);

    // printMatrix(direct_edges, len, len, "Direct edges: ");
    // printMatrix(access_edges, len, len, "Access edges: ");
    // printMatrix(hub_edges, len, len, "Hub edges: ");

    // free_int_matrix(direct_edges, len);
    // free_int_matrix(access_edges, len);
    // free_int_matrix(hub_edges, len);
}

void get_flow_usage(FEDGE **edges, int len)
{
    int access_edges = 0;
    int hub_edges = 0;
    int direct_edges = 0;
    int hub_flow = 0;
    int direct_flow = 0;
    double hub_pct = 0.0;
    double direct_pct = 0.0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if(edges[i][j].is_hub_edge)
            {
                hub_edges++;
                hub_flow += edges[i][j].w;
            }
            else if (edges[i][j].is_access_edge)
            {
                access_edges++;
                hub_flow += edges[i][j].w;
            }
            else if (edges[i][j].is_direct)
            {
                direct_edges++;
                direct_flow += edges[i][j].w;
            }
        }
    }
    hub_pct = hub_flow * 1.0 / (hub_flow + direct_flow);
    direct_pct = 1 - hub_pct;

    printf("Direct_flow: %5.2f, hub_flow: %5.2f, Direct Edges: %d, Access Edges: %d\n", direct_pct, hub_pct, direct_edges, access_edges);
}

void clone_edge_matrix(FEDGE **origin, FEDGE **target, int len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            target[i][j].i = origin[i][j].i;
            target[i][j].j = origin[i][j].j;
            target[i][j].t = origin[i][j].t;
            target[i][j].sp = origin[i][j].sp;
            target[i][j].w = origin[i][j].w;
            target[i][j].is_access_edge = origin[i][j].is_access_edge;
            target[i][j].is_exit_edge = origin[i][j].is_exit_edge;
            target[i][j].is_direct = origin[i][j].is_direct;
            target[i][j].is_hub_edge = origin[i][j].is_hub_edge;
            target[i][j].has_saving = origin[i][j].has_saving;
        }
    }
}

void destroy_edges(FEDGE **edges, int len)
{
    for (int i = 0; i < len; i++)
        free(edges[i]);
    free(edges);
}

FNODE *create_node_list(int len)
{
    return (FNODE *)calloc(len, sizeof(FNODE));
}

void populate_node_list(FNODE *node_list, int len, int **t, int **w)
{
    for (int i = 0; i < len; i++)
    {
        node_list[i].ratio = calculate_node_ratio(i, len, t, w);
        node_list[i].n = i;
    }
}

double calculate_node_ratio(int i, int len, int **t, int **w)
{
    int acum = 0;
    for (int j = 0; j < len; j++)
    {
        acum += t[i][j] * w[i][j];
    }
    return acum/len;
}

int compare_nodes_desc(const void *a, const void *b)
{
    if (((FNODE *)a)->ratio > ((FNODE *)b)->ratio)
        return -1;
    if (((FNODE *)a)->ratio < ((FNODE *)b)->ratio)
        return 1;
    return 0;
}
int compare_nodes_asc(const void *a, const void *b)
{
    if (((FNODE *)a)->ratio > ((FNODE *)b)->ratio)
        return 1;
    if (((FNODE *)a)->ratio < ((FNODE *)b)->ratio)
        return -1;
    return 0;
}

void sort_nodes(FNODE *node_list, int len, int order)
{
    if (order)
        qsort(node_list, len, sizeof(FNODE), compare_edges_asc);
    else
    {
        qsort(node_list, len, sizeof(FNODE), compare_edges_desc);
    }
}

double get_max_node_ratio(FNODE *node_list, int len)
{
    double max_ratio = 0;
    for (int i = 0; i < len; i++)
        max_ratio = node_list[i].ratio > max_ratio ? node_list[i].ratio : max_ratio;

    return max_ratio;
}

double get_min_node_ratio(FNODE *node_list, int len)
{
    double min_ratio = 0;
    for (int i = 0; i < len; i++)
        min_ratio = node_list[i].ratio > min_ratio ? node_list[i].ratio : min_ratio;

    return min_ratio;
}

int compare_edges_desc(const void *a, const void *b)
{
    if (((FEDGE *)a)->ratio > ((FEDGE *)b)->ratio)
        return -1;
    if (((FEDGE *)a)->ratio < ((FEDGE *)b)->ratio)
        return 1;
    return 0;
}
int compare_edges_asc(const void *a, const void *b)
{
    if (((FEDGE *)a)->ratio > ((FEDGE *)b)->ratio)
        return 1;
    if (((FEDGE *)a)->ratio < ((FEDGE *)b)->ratio)
        return -1;
    return 0;
}
void sort_edges(FEDGE *edge_list, int len, int order)
{
    if (order)
        qsort(edge_list, len * len, sizeof(FEDGE), compare_edges_asc);
    else
    {
        qsort(edge_list, len * len, sizeof(FEDGE), compare_edges_desc);
    }
}
double calculate_edge_ratio(int i, int j)
{
    return f_i[i] / max_f_i;
}

FEDGE *create_edge_list(int len)
{
    return (FEDGE *)calloc(len * len, sizeof(FEDGE));
}

void populate_edge_list(FEDGE *edge_list, int len)
{
    int k = 0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            edge_list[k].ratio = calculate_edge_ratio(i, j);
            edge_list[k].i = i;
            edge_list[k].j = j;
            k++;
        }
    }
}