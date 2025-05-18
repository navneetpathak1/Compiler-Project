

#include "astDef.h"

typedef struct node {
  ASTNode* v;
  char* parent;
  int depth;
  struct node* next;
}node;

typedef struct Queue {
  node* head;
  node* tail;
  int size;
}Queue;
