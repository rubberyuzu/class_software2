#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 75
#define HEIGHT 50

const double G = 1.0;  // gravity constant

struct star
{
  double m;   // mass
  double x;   // position_x
  double vx;  // velocity_x
};

struct star stars[] = {
  { 1.0, -10.0, 0.0 },
  { 0.5,  10.0, 0.2 } 
};

const int nstars = sizeof(stars) / sizeof(struct star);
// 全体のbyte数 ... sizeof(stars) 

void plot_stars(FILE *fp, const double t)
{
  int i;
  char space[WIDTH][HEIGHT];
  // space ... the starting address

  memset(space, ' ', sizeof(space));
  // from 'space' address, fill the sizeof(space) bytes
  // all with ' 's

  for (i = 0; i < nstars; i++) {
    const int x = WIDTH  / 2 + stars[i].x;
    const int y = HEIGHT / 2;
    if (x < 0 || x >= WIDTH)  continue;
    if (y < 0 || y >= HEIGHT) continue;
    char c = 'o';
    if (stars[i].m >= 1.0) c = 'O';
    space[x][y] = c;
  }

  int x, y;
  fprintf(fp, "----------\n");
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++)
      fputc(space[x][y], fp);
    fputc('\n', fp);
  }
  fflush(fp);

  printf("t = %5.1f", t);
  for (i = 0; i < nstars; i++) 
    printf(", stars[%d].x = %7.2f", i, stars[i].x);
  printf("\n");

  usleep(200 * 1000);
  // usleep ... more detailed sleep for a shorter time duration
}

void update_velocities(const double dt)
{
  int i, j;
  for (i = 0; i < nstars; i++) {
    double ax = 0;
    for (j = 0; j < nstars; j++) {
      if (i == j) continue;
      const double dx = stars[j].x - stars[i].x;
      const double r = fabs(dx);
      ax += stars[j].m * dx / (r * r * r);
    }
    ax *= G;
    stars[i].vx += ax * dt;
  }
}

void update_positions(const double dt)
{
  int i;
  for (i = 0; i < nstars; i++) {
    stars[i].x += stars[i].vx * dt;
  }
}

int main(int argc, char *argv[])
{
  const char *space_file = "space.txt";
  FILE *fp;
  if ((fp = fopen(space_file, "a")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", space_file);
    return 1;
  }

  // const double dt = 1.0;
  char* dt_char = argv[1];
  const double dt = atof(dt_char);
  const double stop_time = 400;

  int i;
  double t;
  for (i = 0, t = 0; t <= stop_time; i++, t += dt) {
    update_velocities(dt);
    update_positions(dt);
    if (i % 10 == 0) {
      plot_stars(fp, t);
      // only once in 10 timess plotting stars
    }
  }

  fclose(fp);

  return 0;
}