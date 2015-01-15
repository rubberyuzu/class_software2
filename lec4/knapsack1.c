#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_NITEMS 1000
#define MAX_WEIGHT 15

const double W = 20;      // capacity of the knapsack
double v[MAX_NITEMS + 1]; // value
double w[MAX_NITEMS + 1]; // weight
int x[MAX_NITEMS + 1];    // 0 (out) or 1 (in)

void print_solution(const int n, const int _x[MAX_NITEMS + 1])
{
  int i;
  double sum_value = 0;
  double sum_weight = 0;
  for (i = 1; i <= n; i++) {
    printf("%d", _x[i]);
    if (_x[i] == 0) continue;
    sum_value  += v[i];
    sum_weight += w[i];
  }
  printf(", total_value = %5.1f, total_weight = %5.1f\n", sum_value, sum_weight);
}

double search(const int i, const int n, const double sum_v, const double sum_w)
{
  assert(i >= 1 && sum_v >= 0 && sum_w >= 0);
  if (i == n + 1) {
    if(sum_w > MAX_WEIGHT){
      return 0;
    }
    print_solution(n, x);
    return sum_v;
  }

  x[i] = 0;
  double v0 = search(i + 1, n, sum_v, sum_w);

  x[i] = 1;
  double v1 = search(i + 1, n, sum_v + v[i], sum_w + w[i]);

  return (v0 > v1) ? v0 : v1;
}

double solve(const int n)
{
  double value = search(1, n, 0, 0);

  return value;
}

int main(int argc, char **argv)
{
  int i;

  if (argc != 2) {
    fprintf(stderr, "error: please specify the number of items.\n");
    exit(1);
  }

  const int n = atoi(argv[1]);
  assert(n <= MAX_NITEMS);

  printf("W = %.1f\n", W);

  // set values and weights randomly
  for (i = 1; i <= n; i++) {
    v[i] = 0.1 * (rand() % 200);
    w[i] = 0.1 * (rand() % 200);
    x[i] = 0;
    printf("v[%d] = %4.1f, w[%d] = %4.1f\n", i, v[i], i, w[i]);
  }
  printf("-----\n");

  double value = solve(n);

  printf("-----\nbest solution:\n");
  printf("value: %f\n", value);

  return 0;
}