// ３次元拡張 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 75
#define HEIGHT 50
#define DEPTH 50

const double G = 1.0;  // gravity constant

struct star
{
  double m;   // mass
  double x;   // position_x
  double y;
  double z;
  double vx;  // velocity_x
  double vy;  // velocity_y
  double vz; 
};

struct star stars[] = {
  { 1.0, -10.0, -10.0, -10.0, 0.0, 0.0, 1.2},
  { 0.5,  10.0, 10.0, 10.0, 0.2, 0.2, 0.5}, 
  { 0.2,  0.0, 0.0, 0.0, 0.3, 0.3, 1.0 }
};

int nstars = sizeof(stars) / sizeof(struct star);
// 全体のbyte数 ... sizeof(stars) 

void plot_stars(FILE *fp, const double t)
{
  int i;
  char space[WIDTH][HEIGHT][DEPTH];
  // space ... the starting address

  memset(space, ' ', sizeof(space));
  // from 'space' address, fill the sizeof(space) bytes
  // all with ' '

  for (i = 0; i < nstars; i++) {
    const int x = WIDTH  / 2 + stars[i].x;
    const int y = HEIGHT / 2 + stars[i].y;
    const int z = DEPTH / 2 + stars[i].z;
    if (x < 0 || x >= WIDTH)  continue;
    if (x < 0 || x >= WIDTH)  continue;
    if (z < 0 || z >= DEPTH) continue;
    char c = 'o';
    if (stars[i].m >= 1.0) c = 'O';
    space[x][y][z] = c;
  }

  int x, y;
  fprintf(fp, "----------\n");
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++){
      for (z = 0; z < DEPTH; ++i)
      fputc(space[x][y], fp);
      fputc('\n', fp);
    }
  }
  fflush(fp);

  printf("t = %5.1f", t);
  for (i = 0; i < nstars; i++) 
    printf(", stars[%d].x = %7.2f", i, stars[i].x);
    // printf(", stars[%d].y = %7.2f", i, stars[i].y);
  printf("\n");

  usleep(200 * 1000);
  // usleep ... more detailed sleep for a shorter time duration
}

void update_velocities(const double dt)
{
  int i, j;
  for (i = 0; i < nstars; i++) {
    double ax = 0;
    double ay = 0;
    double az = 0;
    for (j = 0; j < nstars; j++) {
      if (i == j) continue;
      const double dx = stars[j].x - stars[i].x;
      const double dy = stars[j].y - stars[i].y;
      const double dy = stars[j].z - stars[i].z;
      const double r = fabs(dx);
      ax += stars[j].m * dx / (r * r * r);
      ay += stars[j].m * dy / (r * r * r);
      az += stars[j].m * dz / (r * r * r);
    }
    ax *= G;
    ay *= G;
    az *= G;
    stars[i].vx += ax * dt;
    stars[i].vy += ay * dt;
    stars[i].vz += az * dt;

  }
}

void update_positions(const double dt)
{
  int i;
  for (i = 0; i < nstars; i++) {
    stars[i].x += stars[i].vx * dt;
    stars[i].y += stars[i].vy * dt;
    stars[i].z += stars[i].vz * dt;

  }
}

double distance(int i, int j){
  return sqrt(pow(stars[i].x-stars[j].x , 2)+pow(stars[i].y-stars[j].y , 2)+pow(stars[i].z-stars[j].z , 2));
}


void collision(const double dt)
{
  int i, j;
  for (i = 0; i < nstars; i++) {
    if (i == nstars-1){
      return;
    }
    for (j = i+1; j < nstars; ++j){
      if (distance(i, j) < 10 ){
        stars[i].m += stars[j].m;
        stars[i].vx = (stars[i].m * stars[i].vx + stars[j].m * stars[j].vx);
        stars[i].vy = (stars[i].m * stars[i].vy + stars[j].m * stars[j].vy);
        stars[i].vz = (stars[i].m * stars[i].vz + stars[j].m * stars[j].vz);

        while(j < nstars-1){
          stars[j] = stars[j+1];
          j++;
        }
        nstars -=1 ;
        printf("nstars:  %d\n", nstars);
      }
    }
  }
}


int main()
{
  const char *space_file = "space.txt";
  FILE *fp;
  if ((fp = fopen(space_file, "a")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", space_file);
    return 1;
  }

  const double dt = 1.0;
  const double stop_time = 400;

  int i;
  double t;
  for (i = 0, t = 0; t <= stop_time; i++, t += dt) {
    update_velocities(dt);
    update_positions(dt);
    collision(dt);
    if (i % 10 == 0) {
      plot_stars(fp, t);
      // only once in 10 timess plotting stars
    }
  }

  fclose(fp);

  return 0;
}