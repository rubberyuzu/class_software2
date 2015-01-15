#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#define WIDTH 70
#define HEIGHT 40
#define MAX_NCITIES 100
#define MAX_DISTANCE 1000000
#define MAX_ROUTES 1000000;

int perm_arr[1000000][100];
int ind = 0;

struct city
{
  int x;
  int y;
};

typedef struct city City;

City cities[MAX_NCITIES + 1];
char map[WIDTH][HEIGHT];

int max(int a, int b)
{
  return (a > b) ? a : b;
}

void draw_line(int x0, int y0, int x1, int y1)
{
  int i;
  int n = max(abs(x1 - x0), abs(y1 - y0));
  for (i = 0; i <= n; i++) {
    const int x = x0 + i * (x1 - x0) / n;
    const int y = y0 + i * (y1 - y0) / n;
    if (map[x][y] == ' ') map[x][y] = '*';
  }
}

void draw_route(const int n, const int *route)
{
  if (route == NULL) return;

  int i;
  for (i = 1; i <= n; i++) {
    const int c0 = route[i];
    const int c1 = (i < n) ? route[i+1] : route[1];
    draw_line(cities[c0].x, cities[c0].y, cities[c1].x, cities[c1].y);
  }
}

void plot_cities(FILE *fp, const int n, const int *route)
{
  int i, j, x, y;

  fprintf(fp, "----------\n");

  memset(map, ' ', sizeof(map));

  for (i = 1; i <= n; i++) {
    char buf[100];
    sprintf(buf, "C_%d", i);
    for (j = 0; j < strlen(buf); j++) {
      const int x = cities[i].x + j;
      const int y = cities[i].y;
      map[x][y] = buf[j];
    }
  }

  draw_route(n, route);
    
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
      const char c = map[x][y];
      fputc(c, fp);
    }
    fputc('\n', fp);
  }

  fflush(fp);
}

double distance(const int i, const int j)
{
  const double dx = cities[i].x - cities[j].x;
  const double dy = cities[i].y - cities[j].y;
  return sqrt(dx * dx + dy * dy);
}

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
  int perm[size];
  put(v, size, perm);
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
  if(ind==0){
    print(v, n);
    int perm[n];
    int i;
    int* result = perms(v, n, perm);
    for (i = 0; i < n; ++i)
    {
      perm_arr[ind][i] = result[i];
    }
    ind++;
  }else{
    print(v, n);
    int perm[n];
    int i;
    int* result = perms(v, n, perm);
    for (i = 0; i < 6; ++i)
    {
      perm_arr[ind][i] = result[i];
    }
    ind++;


  }

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

int factorial(int f)
{
    if ( f == 0 ) 
        return 1;
    return(f * factorial(f - 1));
}

double solve(const int n, int route[])
{
  int i;
  int *v = (int*) malloc(sizeof(int)*n);
  init(v, n);
  permute(v, 0, n);

  int num_of_routes = factorial(n);
  int num_index;

  int best_route[MAX_NCITIES];

  int min_distance = MAX_DISTANCE;

  for (num_index = 0; num_index < num_of_routes; ++num_index)
  {
    double sum_d = 0;

    for (i = 1; i <= n; i++) {
      route[i] = perm_arr[num_index][i-1] ;
    }

    for (i = 1; i <= n; i++) {
      if(i != n){
        const int c0 = route[i];
        const int c1 = route[i+1];
        sum_d += distance(c0, c1);
      }else{
        const int c0 = route[i];
        const int c1 = route[1];
        sum_d += distance(c0, c1);
      }
    }
    if(min_distance > sum_d){
      min_distance = sum_d;

      for (i = 1; i <= n; i++) {
        best_route[i] = route[i];
      }
    }

  }


  free(v);
  for (i = 1; i <= n; i++) {
    route[i] = best_route[i];
  }

  return min_distance;
}

int main(int argc, char **argv)
{
  int i;
  FILE *fp;
  const char *map_file = "map.txt";

  if ((fp = fopen(map_file, "a")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", map_file);
    return 1;
  }

  if (argc != 2) {
    fprintf(stderr, "error: please specify the number of cities.\n");
    exit(1);
  }

  const int n = atoi(argv[1]);
  assert(n > 1 && n <= MAX_NCITIES);

  for (i = 1; i <= n; i++) {
    cities[i].x = rand() % (WIDTH - 5);
    cities[i].y = rand() % HEIGHT;
  }

  plot_cities(fp, n, NULL);
  sleep(1);

  int route[MAX_NCITIES];
  const double d = solve(n, route);
  
  printf("total distance = %f\n", d);

  for (i = 1; i <= n; ++i)
  {
    printf("route[%d] = %d\n",i, route[i] );
  }

  plot_cities(fp, n, route);


  return 0;

}