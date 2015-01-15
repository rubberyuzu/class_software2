#include <stdio.h>
#include <stdlib.h>

/**
   Read a number, N, from standard input and print the
   permutations.
 */

void put(const int *v, const int size, int perm[])
{
  if (v != 0) {
    for (int i = 0; i < size; i++) {
      perm[i] = v[i];
    }
  }
} // put


void print(const int *v, const int size)
{


  if (v != 0) {
    for (int i = 0; i < size; i++) {
      printf("%4d", v[i] );
    }
    printf("\n");
  }
} // print

int* perms(const int *v, const int size, int* perm)
{
  put(v, size, perm);
  printf("perm[0] = %d\n", perm[0]);
  return perm;
} // print



void swap(int *v, const int i, const int j)
{
  int t;
  t = v[i];
  v[i] = v[j];
  v[j] = t;
}


void rotateLeft(int *v, const int start, const int n)
{
  int tmp = v[start];
  for (int i = start; i < n-1; i++) {
    v[i] = v[i+1];
  }
  v[n-1] = tmp;
} // rotateLeft


void permute(int *v, const int start, const int n)
{

  print(v, n);
  int perm[n];
  perms(v, n, perm);


  if (start < n) {
    int i, j;
    for (i = n-2; i >= start; i--) {
      for (j = i + 1; j < n; j++) {
    swap(v, i, j);
    permute(v, i+1, n);
      } // for j
      rotateLeft(v, i, n);
    } // for i
  }
} // permute


void init(int *v, int N)
{
  for (int i = 0; i < N; i++) {
    v[i] = i+1;
  }
} // init


int main()
{
    int *v = (int*) malloc(sizeof(int)*10);
    init(v, 3);
    permute(v, 0, 3);
    free(v);
}