#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000

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

int main()
{
  Node *begin = NULL, *p;

  char buf[MAXLEN];
  while (fgets(buf, MAXLEN, stdin)) {
    begin = push_back(begin, buf);
  }

  begin = pop_front(begin);
  begin = pop_back(begin);
  begin = pop_back(begin);
  begin = pop_back(begin);

  for (p = begin; p != NULL; p = p->next) {
    printf("%s", p->str);
  }
  
  return 0;
}