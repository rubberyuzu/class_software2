#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define HEIGHT 50
#define WIDTH 70

int cell[HEIGHT][WIDTH];

void init_cells()
{
  int i, j;

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = rand() % 2;
    }
  }
}

void print_cells(FILE *fp)
{
  int i, j;

  fprintf(fp, "----------\n");

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      char c = (cell[i][j] == 1) ? '#' : ' ';
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);

  sleep(1);
}

int count_adjacent_cells(int i, int j)
{
  int n = 0;
  int k, l;
  for (k = i - 1; k <= i + 1; k++) {
    if (k < 0 || k >= HEIGHT) continue;
    for (l = j - 1; l <= j + 1; l++) {
      if (k == i && l == j) continue;
      if (l < 0 || l >= WIDTH) continue;
      n += cell[k][l];
    }
  }
  return n;
}

void update_cells()
{
  int i, j;
  int cell_next[HEIGHT][WIDTH];

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      int n = count_adjacent_cells(i, j);
      if (cell[i][j] == 0) {
	if (n == 3)  
	  cell_next[i][j] = 1;
	else         
	  cell_next[i][j] = 0;
      } else {
	if (n == 2 || n == 3)  
	  cell_next[i][j] = 1;
	else 	  
	  cell_next[i][j] = 0;
      }
    }
  }

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = cell_next[i][j];
    }
  }
}


int main()
{
  int gen = 1;

  FILE *fp;

  if ((fp = fopen("cells.txt", "w")) == NULL) {
    fprintf(stderr, "error: cannot open a file.\n");
    return 1;
  }

  init_cells();
  print_cells(fp);

  while (1) {
    printf("generation = %d\n", gen++);
    update_cells();
    print_cells(fp);
  }

  fclose(fp);
}