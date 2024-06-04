#include "splay_tree.h"

#define COUNT 6
int main(void) {
  Tree splay = {0};
  insert_many(&splay, COUNT, (KEY[COUNT]){7, 4, 8, 5, 6, 3});
  search(&splay, 7);
  print(&splay);
}
