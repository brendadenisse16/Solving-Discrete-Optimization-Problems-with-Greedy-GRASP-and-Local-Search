#include "def.h"

//FEDGE **edges;
//int *z, I, *h;
int **t, **w;
double C, alpha, betha, t_a, t_e, r_ij; //program general parameters
// local functions declaration
// int shortestPath(int **dist, int k, int i, int j);
int min_path(int i, int j, int n, int **dist, int *h, int **vert);
//void pr(int x, int y, int **vert);
bool isAccessVertex(int i, int j, int *z);
void update_edges(int **dist, int **vert, int len, int *z, FEDGE **edges);

//this method calculates the shortest path but using the selected hubs (z) as intermediate
int **floydWarshallMod(int **graph, int *z, int n_nodes, int p, int *h, FEDGE **edges_or)
{
    FEDGE **edges = initialize_edge_matrix(t, w, n_nodes);
    int **dist = copy_int_matrix(graph, n_nodes, n_nodes);
    int **vert = create_and_initialize_int_matrix(n_nodes, n_nodes, -1);

    int **visited = create_and_initialize_int_matrix(n_nodes, n_nodes, 0);

    int hub_edge_count = 0;
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < p; j++)
        {
            int orig = h[i];
            int dest = h[j];
            if (orig != dest && visited[orig][dest] == 0)
            {
                int mp = min_path(orig, dest, p - 1, dist, h, vert);
                if (mp < dist[orig][dest])
                {
                    dist[orig][dest] = INF;
                    dist[orig][dest] = INF;
                }
                else
                {
                    if (hub_edge_count >= (p - 1))
                    {
                        dist[orig][dest] = INF;
                        break;
                    }
                    dist[orig][dest] = mp;
                    edges[orig][dest].is_hub_edge = true;
                    edges[orig][dest].is_direct = false;
                    edges[orig][dest].sp = dist[orig][dest];
                    dist[orig][dest] = mp;
                    edges[dest][orig].is_hub_edge = true;
                    edges[dest][orig].is_direct = false;
                    edges[dest][orig].sp = dist[dest][orig];
                    hub_edge_count++;
                }
            }
            visited[orig][dest] = 1;
            visited[dest][orig] = 1;
        }
        if (hub_edge_count >= (p - 1))
            break;
    }
    // printf("\nhub_edge_count: %d\n", hub_edge_count);

    for (int i = 0; i < n_nodes; i++)
    {
        for (int j = 0; j < n_nodes; j++)
        {
            if (i != j && visited[i][j] == 0)
            {
                dist[i][j] = min_path(i, j, p - 1, dist, h, vert);
            }
            visited[i][j] = 1;
            visited[j][i] = 1;
        }
    }
    update_edges(dist, vert, n_nodes, z, edges);
    clone_edge_matrix(edges, edges_or, n_nodes);
    destroy_edges(edges, n_nodes);
    free_int_matrix(vert, n_nodes);
    return dist;
}

int min_path(int i, int j, int n, int **dist, int *h, int **vert)
{
    int k = h[n];
    if (n == 0)
    {
        int res = min(dist[i][j], dist[i][k] + dist[k][j]);
        if (res < dist[i][j])
        {
            vert[i][j] = k;
            vert[j][i] = k;
        }
        return res;
    }
    else
    {
        int a = min_path(i, j, n - 1, dist, h, vert);
        int b = min_path(i, k, n - 1, dist, h, vert) + min_path(k, j, n - 1, dist, h, vert);
        if (b < a)
        {
            vert[i][j] = k;
            vert[j][i] = k;
        }

        return min(a, b);
    }
}

void update_edges(int **dist, int **vert, int len, int *z, FEDGE **edges)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {

            if (dist[i][j] != INF && dist[i][j] > 0)
            { //exist a path between the two nodes
                if (vert[i][j] != -1)
                {
                    //i -> k
                    if (isAccessVertex(i, vert[i][j], z))
                    {
                        edges[i][vert[i][j]].is_access_edge = true;
                        edges[i][vert[i][j]].is_exit_edge = true;
                        edges[i][vert[i][j]].is_direct = false;
                        edges[i][vert[i][j]].sp = dist[i][vert[i][j]];
                    }
                    //k -> j
                    if (isAccessVertex(vert[i][j], j, z))
                    {
                        edges[vert[i][j]][j].is_access_edge = true;
                        edges[vert[i][j]][j].is_exit_edge = true;
                        edges[vert[i][j]][j].is_direct = false;
                        edges[vert[i][j]][j].sp = dist[vert[i][j]][j];
                    }
                }
                else if (isAccessVertex(i, j, z))
                {
                    edges[i][j].is_access_edge = true;
                    edges[i][j].is_exit_edge = true;
                    edges[i][j].is_direct = false;
                    edges[i][j].sp = dist[i][j];
                    edges[j][i].is_access_edge = true;
                    edges[j][i].is_exit_edge = true;
                    edges[j][i].is_direct = false;
                    edges[j][i].sp = dist[i][j];
                }
                else
                {
                    if (!(isHub(z, i) && isHub(z, j)))
                    {
                        edges[i][j].is_access_edge = false;
                        edges[i][j].is_exit_edge = false;
                        edges[i][j].is_direct = true;
                        edges[i][j].sp = edges[i][j].t;
                    }
                }
            }
        }
    }
}
bool isAccessVertex(int i, int j, int *z)
{
    return ((isHub(z, i) && !isHub(z, j)) || (!isHub(z, i) && isHub(z, j)));
}

void floydWarshallMod2(int **graph, int *z, int n_nodes, int p, int *h, int **dist, int **vert)
{
    int **visited = create_and_initialize_int_matrix(n_nodes, n_nodes, 0);
    clone_int_matrix(graph, dist, n_nodes, n_nodes);
    int *hub_edge_count = create_and_initialize_int_vector(n_nodes, 0);
    int total_hub_edges = 0;
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < p; j++)
        {
            int orig = h[i];
            int dest = h[j];

            if (orig != dest && (visited[orig][dest] == 0 || visited[dest][orig] == 0))
            {
                //not visited pair
                if (total_hub_edges >= (p - 1))
                {
                    dist[orig][dest] = INF;
                }
                int mp = min_path(orig, dest, p - 1, dist, h, vert);
                if (mp < dist[orig][dest])
                { // por k
                    dist[orig][dest] = INF;
                    dist[dest][orig] = INF;
                }
                else if (hub_edge_count[orig] < 2 && hub_edge_count[dest] < 2)
                { //directo
                    hub_edge_count[orig] += 1;
                    hub_edge_count[dest] += 1;

                    vert[orig][dest] = orig; //marcamos como sp
                    vert[dest][orig] = dest; //marcamos como sp
                    dist[orig][dest] = mp;
                    total_hub_edges++;
                }
            }
            visited[orig][dest] = 1;
            visited[dest][orig] = 1;
        }
    }
    //printMatrix(dist, n_nodes, n_nodes, "dist: ");
    // printf("\total_hub_edges: %d\n", total_hub_edges);

    for (int i = 0; i < n_nodes; i++)
    {
        for (int j = 0; j < n_nodes; j++)
        {
            if (i != j)
            {
                dist[i][j] = min_path(i, j, p - 1, dist, h, vert);
                dist[j][i] = dist[i][j];
            }
            visited[i][j] = 1;
            visited[j][i] = 1;
        }
    }
    //printMatrix(dist, n_nodes, n_nodes, "dist 2: ");
}

bool isHub(int *z, int k)
{
    return z[k] == 1;
}
