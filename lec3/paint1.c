#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define WIDTH 70
#define HEIGHT 40

#define HISTORY_SIZE 100
#define BUFSIZE 1000

char canvas[WIDTH][HEIGHT];
char history[HISTORY_SIZE][BUFSIZE];

void print_canvas(FILE *fp)
{
  int x, y;

  fprintf(fp, "----------\n");

  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
      char c = canvas[x][y];
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);
}

void init_canvas()
{
  memset(canvas, ' ', sizeof(canvas));
}

int max(int a, int b)
{
  return (a > b) ? a : b;
}

void draw_line(int x0, int y0, int x1, int y1)
{
  int i;
  int n = max(abs(x1 - x0), abs(y1 - y0));
  for (i = 0; i <= n; i++) {
    int x = x0 + i * (x1 - x0) / n;
    int y = y0 + i * (y1 - y0) / n;
    canvas[x][y] = '#';
  }
}

void draw_rectangle(int x0, int y0, int h, int w)
{
  int height;
  int width;

  for (height= 0; height < h; ++height)
  {
    int y = y0 + height;
    for (width = 0; width < w; ++width)
    {
      int x = x0 + width;
      if(x>WIDTH||y>HEIGHT){
        continue;
      }
      canvas[x][y] = '#';
    }
  }
}

void draw_circle(int x0, int y0, int r)
{
  int i;
  for (i = 0; i < 360; ++i)
  {
    int x = floor(x0+r*cos(i));
    int y = floor(y0+r*sin(i));
    if(x>WIDTH||y>HEIGHT){
      continue;
    }
    canvas[x][y] = '#';
  }
}

int interpret_command(const char *command)
{
  char buf[BUFSIZE];
  strcpy(buf, command);

  char *s = strtok(buf, " ");
  char c = tolower(s[0]);

  switch (c) {
  case 'l': {
    int x0, y0, x1, y1, i, n;
    x0 = atoi(strtok(NULL, " "));
    y0 = atoi(strtok(NULL, " "));
    x1 = atoi(strtok(NULL, " "));
    y1 = atoi(strtok(NULL, " "));
    draw_line(x0, y0, x1, y1);
    break;
  }
  case 'c': {
    int x0, y0, r;
    x0 = atoi(strtok(NULL, " "));
    y0 = atoi(strtok(NULL, " "));
    r = atoi(strtok(NULL, " "));
  
    draw_circle(x0, y0, r);
    break;
  }
  case 'r': {
    int x0, y0, h, w;
    x0 = atoi(strtok(NULL, " "));
    y0 = atoi(strtok(NULL, " "));
    h = atoi(strtok(NULL, " "));
    w = atoi(strtok(NULL, " "));
    draw_rectangle(x0, y0, h, w);
    break;
  }
  case 'u':
    return 1;
    break;
  case 'q':
    return -1;
  default:
    break;
  }

  return 0;
}

int main()
{
  int i;
  const char *canvas_file = "canvas.txt";
  FILE *fp;
  char buf[BUFSIZE];

  if ((fp = fopen(canvas_file, "a")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", canvas_file);
    return 1;
  }

  init_canvas();
  print_canvas(fp);

  for (i = 0; i < HISTORY_SIZE; i++) {

    printf("%d > ", i);
    fgets(buf, BUFSIZE, stdin);
    strcpy(history[i], buf);
    //strcpy(&(history[i][0]), buf);

    int r = interpret_command(buf);
    if (r == -1) break;
    if (r == 1) { // undo
      int j;
      init_canvas();
      for (j = 0; j < i - 1; j++) {
	       interpret_command(history[j]);
      }
      i -= 2;
    }

    print_canvas(fp);
  }

  fclose(fp);

  return 0;
}