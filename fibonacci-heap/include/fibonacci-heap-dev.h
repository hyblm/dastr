#include "fibonacci-heap.h"
#include <stdbool.h>

struct Node {
  KEY key;
  node_t *parent;
  node_t *sibling_prev;
  node_t *sibling_next;
  node_t *child;
  uint8_t degree;
  bool marked;
};

void print_key(char *prefix, node_t *node, char *sufix);
