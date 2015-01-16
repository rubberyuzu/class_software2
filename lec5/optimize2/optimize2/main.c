#include <stdio.h>
#include <stdlib.h>
#include "optimize2.h"
#include "func.h"

int main(int argc, char **argv)
{
  const double alpha = (argc == 2) ? atof(argv[1]) : 0.01;

  int i;
  const int dim = f_dimension(); // return 2

  double *x = malloc(dim * sizeof(double));
  for (i = 0; i < dim; i++) {
    x[i] = 0; // initializing an array w 2 elems
  }

  const int iter = optimize(alpha, f_dimension(), x, f_gradient, f_value);

  printf("number of iterations = %d\n", iter);

  free(x);

  return 0;
}

