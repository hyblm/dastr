#include "splay_tree.h"

int main(void) {
  Splay tree = {0};
  insert(&tree, 7);
  insert(&tree, 4);
  insert(&tree, 5);
  insert(&tree, 6);
  insert(&tree, 3);
  search(&tree, 5);
  print(&tree);
}
