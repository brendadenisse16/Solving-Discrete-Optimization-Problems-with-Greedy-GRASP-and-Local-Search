#include "def.h"

int I, J, p;
int **w, **t, *h;
double *f_i, *f_j;
int *z; //node index of hubs
int *y; //arch index of hub archs
extern int **sp;
FEDGE **edges;

void initialize_local_memory(void)
{
  z = create_and_initialize_int_vector(I, 0);
  y = create_and_initialize_int_vector(I * I, 0);
  sp = create_int_matrix(I, I);
  h = create_int_vector(p);
}

void free_local_memory(void)
{
  //limpiar memoria de los vectores usados
  for (int i = 0; i < I; i++)
  {
    free(w[i]);
    free(t[i]);
    free(sp[i]);
    free(edges[i]);
  }
  free(w);
  free(t);
  free(f_i);
  free(f_j);
  free(sp);
  free(edges);
}

int **create_int_matrix(int rows, int Columns) // funcion te devuelve un array two dimensions de enteros , tb hace y lo mas importante
{
  int i;
  int **ptr;
  //reserva memoria //aqui estoy creando la fila pero no tiene columna en el **ptr
  if ((ptr = (int **)calloc(rows, sizeof(int *))) == NULL)
  {                                           //importante esta funcion tb reserva de bloques de memoria, en este caso reserva cantidad d filas d memoria
    printf("\nError: Insuficient memory \n"); // (int *) reservando espacio pa arreglos de #'s
    exit(8);
  }                                      //creando el arreglo
  for (i = 0; i < rows; i++)             //aqui por cada fila voy creando las columnas y pa esto uso la funcion create_int_vector(Columns)
    ptr[i] = create_int_vector(Columns); //esta funcion crea otra funcion, creando columnas x cada fila
  return ptr;
} //finalmente creo mi arreglo bidimensional, para esto llamo a la  funcion create_int_vector (column) para crear la dimension columna

int **create_and_initialize_int_matrix(int rows, int Columns, int val) // funcion te devuelve un array two dimensions de enteros , tb hace y lo mas importante
{
  int i;
  int **ptr;
  //reserva memoria //aqui estoy creando la fila pero no tiene columna en el **ptr
  if ((ptr = (int **)calloc(rows, sizeof(int *))) == NULL)
  {                                           //importante esta funcion tb reserva de bloques de memoria, en este caso reserva cantidad d filas d memoria
    printf("\nError: Insuficient memory \n"); // (int *) reservando espacio pa arreglos de #'s
    exit(8);
  }                                                          //creando el arreglo
  for (i = 0; i < rows; i++)                                 //aqui por cada fila voy creando las columnas y pa esto uso la funcion create_int_vector(Columns)
    ptr[i] = create_and_initialize_int_vector(Columns, val); //esta funcion crea otra funcion, creando columnas x cada fila

  return ptr;
}

double **create_double_matrix(int rows, int Columns)
{
  int i;
  double **ptr;

  if ((ptr = (double **)calloc(rows, sizeof(double *))) == NULL)
  {
    printf("\nError: Insuficient memory \n");
    exit(8);
  }
  for (i = 0; i < rows; i++)
  {
    ptr[i] = create_double_vector(Columns);
  }
  return ptr;
}
double **create_and_initialize_double_matrix(int rows, int Columns, double val)
{
  int i;
  double **ptr;

  if ((ptr = (double **)calloc(rows, sizeof(double *))) == NULL)
  {
    printf("\nError: Insuficient memory \n");
    exit(8);
  }
  for (i = 0; i < rows; i++)
  {
    ptr[i] = create_and_initialize_double_vector(Columns, val);
  }
  return ptr;
}
//esta funcion es reservar la memoria y crea un arreglo unidimensional
int *create_int_vector(int dim)
{
  int *ptr;

  if ((ptr = (int *)calloc(dim, sizeof(int))) == NULL)
  {
    printf("\nError: Insuficient memory \n");
    exit(8); //8 es un codigo de error de salida
  }
  return ptr;
}

int *create_and_initialize_int_vector(int dim, int val)
{
  int *ptr;

  if ((ptr = (int *)calloc(dim, sizeof(int))) == NULL)
  {
    printf("\nError: Insuficient memory \n");
    exit(8); //8 es un codigo de error de salida
  }
  for (int i = 0; i < dim; i++)
  {
    ptr[i] = val;
  }

  return ptr;
}
double *create_double_vector(int dim)
{
  double *ptr;

  if ((ptr = (double *)calloc(dim, sizeof(double))) == NULL)
  {
    printf("\nError: Insuficient memory \n");
    exit(8);
  }
  return ptr;
}
double *create_and_initialize_double_vector(int dim, double val)
{
  double *ptr;

  if ((ptr = (double *)calloc(dim, sizeof(double))) == NULL)
  {
    printf("\nError: Insuficient memory \n");
    exit(8);
  }
  for (int i = 0; i < dim; i++)
  {
    ptr[i] = val;
  }
  return ptr;
}

// CPLEX functions to allocate memeory to arrays
void i_vector(int **vector, int n, char *s)
{
  if ((*vector = (int *)calloc(n, sizeof(int))) == NULL)
    //error(s);
    printf("Error: Insuficient memory \n");
  return;
}

void d_vector(double **vector, int n, char *s)
{
  if ((*vector = (double *)calloc(n, sizeof(double))) == NULL)
    // error(s);
    printf("Error: Insuficient memory \n");
  return;
}

void c_vector(char **vector, int n, char *s)
{
  if ((*vector = (char *)calloc(n, sizeof(char))) == NULL)
    //error(s);
    printf("Error: Insuficient memory \n");
  return;
}

void printMatrix(int **matrix, int a, int b, char *label)
{
  printf("%s\n", label);
  for (int i = 0; i < a; i++)
  {
    for (int j = 0; j < b; j++)
      printf("%5d", matrix[i][j]);
    printf("\n");
  }
}

void printDoubleMatrix(double **matrix, int a, int b, char *label)
{
  printf("%s\n", label);
  for (int i = 0; i < a; i++)
  {
    for (int j = 0; j < b; j++)
      printf("%5.2f", matrix[i][j]);
    printf("\n");
  }
}

void printVector(int *vector, int l, char *label)
{
  printf("%s\n", label);
  for (int i = 0; i < l; i++)
  {
    printf("%5d", vector[i]);
  }
  printf("\n");
}

int **copy_int_matrix(int **matrix, int i, int j)
{
  int **res = create_int_matrix(i, j);
  for (int x = 0; x < i; x++)
  {
    res[x] = copy_int_vector(matrix[x], j);
  }
  return res;
}
int *copy_int_vector(int *vector, int n)
{
  int *res = create_int_vector(n);
  for (int i = 0; i < n; i++)
    res[i] = vector[i];
  return res;
}

double **copy_double_matrix(double **matrix, int i, int j)
{
  double **res = create_double_matrix(i, j);
  for (int x = 0; x < i; x++)
  {
    res[x] = copy_double_vector(matrix[x], j);
  }
  return res;
}
double *copy_double_vector(double *vector, int n)
{
  double *res = create_double_vector(n);
  for (int i = 0; i < n; i++)
    res[i] = vector[i];
  return res;
}

// stack utils
STACK create_stack(int listLen)
{
  STACK stack;
  stack.top = -1;
  stack.list = create_int_vector(listLen);
  return stack;
}

void clean_stack(STACK *stack)
{
  free(stack->list);
}

bool in_stack(STACK *stack, int elem)
{
  bool res = false;
  for (int i = 0; i < stack->top; i++)
  {
    if (stack->list[i] == elem)
    {
      res = true;
      break;
    }
  }
  return res;
}

void push(int elem, STACK *stack)
{
  if (!in_stack(stack, elem))
  {
    stack->top++;
    stack->list[stack->top] = elem;
  }
}

int pop(STACK *stack)
{
  int res = -1;
  if (stack->top >= 0)
  {
    res = stack->list[stack->top];
    stack->top--;
  }
  return res;
}

void free_int_matrix(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
    free(matrix[i]);
  free(matrix);
}

void remove_from_list(int *list, int len, int pos)
{
  int *new_list;
  int j = 0;

  new_list = create_and_initialize_int_vector(len, -1);
  for (int i = 0; i < len; i++)
  {
    if (i != pos)
      new_list[j++] = list[i];
  }
  list = new_list;
  free(new_list);
}

void shift_in_list(int *list, int len, int pos, int elem)
{
  for (int i = 0; i < len; i++)
  {
    if (i == pos)
      list[i] = elem;
  }
}

// ease the time analysis
double elapsed_time(clock_t start, clock_t end)
{
  return (double)(end - start) / (double)CLOCKS_PER_SEC;
}

void clone_int_vector(int *origin, int *target, int len)
{
  for (int i = 0; i < len; i++)
    target[i] = origin[i];
}

void clone_int_matrix(int **origin, int **target, int n, int m)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      target[i][j] = origin[i][j];
    }
  }
}

void set_int_vector(int *vector, int len, int value)
{
  for (int i = 0; i < len; i++)
    vector[i] = value;
}

void set_int_matrix(int **vector, int m, int n, int value)
{
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      vector[i][j] = value;
    }
  }
}