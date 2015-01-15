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
// char history[HISTORY_SIZE][BUFSIZE];


struct node 
{
  char *str;
  struct node *next;
};

typedef struct node Node;

Node *push_front(Node *begin, const char *str)
{
  Node *p = malloc(sizeof(Node));
  char *s = malloc(strlen(str) + 1);
  strcpy(s, str);
  p->str = s;
  p->next = begin;

  return p;
}

Node *pop_front(Node *begin)
{
  Node *p = begin->next;

  free(begin->str);
  free(begin);

  return p;
}


Node *pop_back(Node *begin)
{
  Node *p = begin;
  Node *q = begin;
  while(p->next != NULL){
    q = p;
    p = p->next;
  }
  free(p->str);
  free(p);
  q->next = NULL;
  return begin;
}

Node *push_back(Node *begin, const char *str)
{
  if (begin == NULL) {
    return push_front(begin, str);
  }

  Node *p = begin;

  while (p->next != NULL) {
    p = p->next;
  }

  Node *q = malloc(sizeof(Node));
  char *s = malloc(strlen(str) + 1);
  strcpy(s, str);
  q->str = s;
  q->next = NULL;

  p->next = q;

  return begin;
}



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

  
  Node *begin = NULL, *p;
  int k = 0;
  printf("%d > ", k);
  while (fgets(buf, BUFSIZE, stdin)) {
    begin = push_back(begin, buf);

    Node *q = malloc(sizeof(Node));
    memcpy(begin, q, sizeof(*begin));
    for (p = begin; p != NULL; p = p->next){
      int r = interpret_command(p->str);
      if (r == -1) break;
      if (r == 1) { // undo
        int j;
        init_canvas();
        begin = pop_back(begin);
        begin = pop_back(begin);
        for (j = 0; j < k - 1; j++) {
           interpret_command(q->str);
           q = q->next;
        }
        k -= 2;
      }
      free(q);
      print_canvas(fp);
      k++;
      printf("%d > ", k);
    }
  }

  

  fclose(fp);

  return 0;
}