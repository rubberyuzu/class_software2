#include <math.h>

int f_dimension()
{
  return 3;
}

double f_value(const double x[])
{
  return (x[0] - 3) * (x[0] - 3)  * (x[0] - 3)  * (x[0] - 3) + (x[1] - 2) * (x[1] - 2) + (x[2] - 4) * (x[2] - 4) * (x[2] - 4) * (x[2] - 4);
}

void f_gradient(const double x[], double g[])
{
  g[0] = 4 * (x[0] - 3) * (x[0] - 3) * (x[0] - 3);
  g[1] = 2 * (x[1] - 2);
  g[2] = 4 * (x[2] - 4) * (x[2] - 4) * (x[2] - 4);
  // もとを、f(x,y,z) = (x-3)^4 + (y-2)^2 + (z-4)^4 にかえる
}

