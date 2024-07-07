#include <stdio.h>
#include "include/fibonacci-heap.h"
int main(void) {
  fib_t* queue = make_fib();

  insert(queue, 3);
  insert(queue, 6);
  insert(queue, 5);
  insert(queue, 2);
  node_t* node = insert(queue, 7);
  insert(queue, 4);
  node_t* node2 = insert(queue, 9);
  insert(queue, 1);
  insert(queue, 12);

  print(queue);

  KEY extracted = extract_min(queue);
  printf("extracted: %d\n", extracted);
  print(queue);

  decrease_key(queue, node, 1);
  print(queue);

  decrease_key(queue, node2, 1);
  print(queue);

  destroy_fib(queue);
  return 0;
}
