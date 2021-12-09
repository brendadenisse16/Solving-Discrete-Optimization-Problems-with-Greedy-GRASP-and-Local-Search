#include "def.h"

// local functions
double calculate_rk(FEDGE **edges, int len);
double calculate_fz(int *h, double *f_i, int p);
double calculate_dk(FEDGE **edges, int len, double alpha, double t_a, double t_e);
double calculate_ec(int **w, FEDGE **edges, int len, double C);
double calculate_ry(FEDGE **edges, double *f_i, int len);

double calculate_total_profit(FEDGE **edges, int len, int **w, int *h, int p, double *f_i, double alpha, double t_a, double t_e, double C)
{
    double rk = calculate_rk(edges, len);
    double dk = calculate_dk(edges, len, alpha, t_a, t_e);
    double fz = calculate_fz(h, f_i, p);
    double ec = calculate_ec(w, edges, len, C);
    double ry = calculate_ry(edges, f_i, len);
    // w(tiempo original - tiempo nuevo ) - costo de instalacion de hub, costo de instalacion de nodo hub - costo de penalizacion por flujo directo
    return rk - dk - fz - ec - ry;
}

double calculate_total_profit2(int *z, int **vert, int **sp, int len, int **t, int **w, double *f_i, double alpha, double t_a, double t_e, double C)
{
    // vertex cost analysis
    double wto, wtsp, fz, ce, ry;
    wto = 0;  //flujo original en red hub * unidad de tiempo
    wtsp = 0; //flujo mejorado en red hub * unidad de tiempo
    fz = 0;
    ce = 0;
    ry = 0;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if (i != j)
            {
                if (vert[i][j] != -1 )
                {
                    if (z[i] == 1 && z[j] == 1 && (vert[i][j] == i || vert[i][j] == j))
                    {
                        wto += t[i][j] * w[i][j];
                        wtsp += sp[i][j] * w[i][j] * alpha;
                        ry += (f_i[i]+ f_i[j]) *1.0  / 2;
                    }
                    //else if is access then
                    else if ((z[i] == 1 || z[j] == 1) && (vert[i][j] == i || vert[i][j] == j))
                    {
                        wto += t[i][j] * w[i][j];
                        wtsp += sp[i][j] * w[i][j] + t_a + t_e;
                    }
                    else if(z[i] != 1 && z[j] != 1)
                    {
                        ce += w[i][j] * C; //flujo de salida penalizado
                    }
                }
                //else vertex doesn't exist;
            }
        }
        if (z[i] == 1)
        {
            fz += f_i[i];
        }
    }
    return wto - wtsp - ry - ce - fz;
}

// tiempo original de nodos de acceso y hub (no directos)
double calculate_rk(FEDGE **edges, int len)
{

    double res = 0;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            res += !edges[i][j].is_direct ? edges[i][j].t * edges[i][j].w : 0;
        }
    }
    return res;
}
// tiempo nuevo con accesos y hubs
double calculate_dk(FEDGE **edges, int len, double alpha, double t_a, double t_e)
{
    // si el edge es directo, el costo es el tiempo * demanda * C (parametro)
    double res = 0.0;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if (!edges[i][j].is_direct)
            {
                if (edges[i][j].is_hub_edge)
                {
                    res += edges[i][j].t * edges[i][j].w * alpha;
                }
                else if (edges[i][j].is_access_edge || edges[i][j].is_exit_edge)
                {
                    res += (edges[i][j].t + t_a) * edges[i][j].w;
                }
            }
        }
    }
    return res;
}

double calculate_fz(int *h, double *f_i, int p)
{
    double fz = 0;

    for (int i = 0; i < p; i++)
    {
        if (i == h[i])
            fz += f_i[i];
    }
    return fz;
}

double calculate_ec(int **w, FEDGE **edges, int len, double C)
{
    double ec = 0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if (edges[i][j].is_direct)
                ec += w[i][j] * C;
        }
    }
    return ec;
}

double calculate_ry(FEDGE **edges, double *f_i, int len)
{
    double res = 0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            res += edges[i][j].is_hub_edge ? (f_i[i] + f_i[j]) / 2 : 0;
        }
    }
    return res;
}