#include "def.h"

extern int I, J/* , maxC */;
/* extern int **c, **a, *b, *c_min, *a_min; */
extern int **w, **t;
extern double *f_i, *f_j, max_f_i, max_f_j;
extern double t_t, r_i_j, t_a, t_e;
extern int *z; //node index of hubs
extern int *y; //arch index of hub archs
extern int p; //number of nodes to be selected
double v; //param related to t_t

// local fuctions definition
FILE *open_file(const char *name, const char *mode);

void read_costs_file(const char *name, int len)
{
  FILE *in;
  f_i = create_double_vector(len);
  max_f_i = 0;
  in = open_file(name, "r");
  for (int i = 0; i < len; i++)
  {
    fscanf(in, "%lf", &f_i[i]);
    if(f_i[i] > max_f_i)
      max_f_i = f_i[i];
  }
}
// Function used to read the input parameters of the instance being solved
void read_data(const char* name)
{
  FILE* in;
  
  in = open_file(name, "r");
  if (fscanf(in, "%d", &I)!=1)//I mantiene el numero de nodos
  {
    fprintf(stderr, "ERROR: Cannot read instance size \n");
    exit(1);
  }
  //inicializo matrices cuadradas
  w = create_int_matrix(I, I);
  t = create_int_matrix(I, I);
  //leo el archivo y populo la matrices w y t
  for (int i = 0; i < I*I; i ++)
  {
    int n_i, n_j, t_ij, w_ij;
    if (fscanf(in,"%d\t%d\t%d\t%d", &n_i, &n_j, &t_ij, &w_ij) < 4) {
      fprintf(stderr, "ERROR: Can't read cost matrix \n");
      exit(1);
    }
    w[n_i - 1][n_j - 1] = w_ij;
    t[n_i - 1][n_j - 1] = t_ij;
  }
}
// Function used to open data file
FILE *open_file(const char *name, const char *mode)
{
  FILE *file;

  if ((file = fopen(name, mode)) == NULL)
  {
    printf("\nError: Failed to open data file \n");
    exit(8);
  }
  return file;
}

void calculate_internals(void)
{
  // creo los vectores calculados f_i, f_j
  // f_i = create_double_vector(I);
  // f_j = create_double_vector(I);
  // max_f_i = 0;
  // max_f_j = 0;
  // for (int i = 0; i < I; i++)
  // {
  //   f_i[i] = 0;
  //   f_j[i] = 0;
  //   for (int j = 0; j < I; j++)
  //   {
  //     f_i[i] += w[i][j];
  //     f_j[i] += w[j][i];
  //   }
  //   f_i[i] = 15 * log(f_i[i]);
  //   f_j[i] = 15 * log(f_j[i]);
  //   if (f_i[i] > max_f_i )
  //     max_f_i = f_i[i];
  //   if (f_j[i] > max_f_j )
  //     max_f_j = f_j[i];
  // }
  // calculo el tiempo de transferencia medio
  t_t = 0.0;
  for (int i = 0; i < I; i++)
  {
    for (int j = 0; j < I; j++)
    {
      t_t += t[i][j];
    }
  }
  t_t = t_t / (I * (I - 1)) * v;
  t_a = 0.1 * t_t; // percentage of t_t assigned to access time
  t_e = t_t - t_a; // percentage of t_t assigned to exit time
}
