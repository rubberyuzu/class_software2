#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "optimize.h"

double calc_norm(const int dim, double v[])
{ // calculating the size of the vector
  double tmp = 0;
  int i = 0;
  for (i = 0; i < dim; i++) {
    tmp += v[i] * v[i];
  }
  tmp = sqrt(tmp);
  return tmp;
}

int optimize(const double alpha, const int dim, double x[], 
	     void (*calc_grad)(const double [], double []))
{
  int i;

  printf("%s\n", "heyheyheyhey");

  double *g = malloc(dim * sizeof(double));

  int iter = 0;
  while (++iter < 10000) {

    (*calc_grad)(x, g); // put gradient in g

    const double norm = calc_norm(dim, g); //size of gradient vector
    printf("%3d norm = %7.4f", iter, norm);
    for (i = 0; i < dim; i++) {
      printf(", x[%d] = %7.4f", i, x[i]);
    }
    putchar('\n');

    if (norm < 0.01) break; // if small enough, end!

    for (i = 0; i < dim; i++) {
      x[i] -= alpha * g[i];
    }
  }

  free(g);

  return iter;
}

