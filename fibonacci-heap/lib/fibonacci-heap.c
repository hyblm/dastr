#include <stdio.h>
#include <stdlib.h>

#include "../include/fibonacci-heap-dev.h"

#define MARKED_MASK 0xf0;

fib_t* make_fib() {
  fib_t* heap = (fib_t*)malloc(sizeof(fib_t));
  if (heap) { *heap = (fib_t){0, NULL}; }

  return heap;
}

node_t* make_node(KEY key, node_t* sibling_prev, node_t* sibling_next) {
  node_t* new_node = (node_t*)malloc(sizeof(node_t));
  if (new_node) {
    if (!sibling_prev) { sibling_prev = new_node; }

    if (!sibling_next) { sibling_next = sibling_prev; }

    *new_node = (node_t){
        .key = key,
        .sibling_next = sibling_next,
        .sibling_prev = sibling_prev,
    };
  }

  new_node->sibling_prev->sibling_next = new_node;
  new_node->sibling_next->sibling_prev = new_node;

  return new_node;
}

void check_update_min(fib_t* heap, node_t* node) {
  if (!node || !heap) { return; }

  if (!heap->min_ptr || (node->key < heap->min_ptr->key)) {
    heap->min_ptr = node;
  }
}

void update_min(fib_t* heap) {
  node_t* node = heap->min_ptr;
  do {
    check_update_min(heap, node);
    node = node->sibling_next;
  } while (node != heap->min_ptr);
}

node_t* insert(fib_t* heap, KEY key) {
  if (!heap->min_ptr) {
    heap->min_ptr = make_node(key, NULL, NULL);
    if (heap->min_ptr) {
      heap->node_count += 1;
      return heap->min_ptr;
    }
    return NULL;
  }

  node_t* new_node = make_node(key, heap->min_ptr->sibling_prev, heap->min_ptr);
  if (!new_node) return NULL;

  heap->node_count += 1;

  if (key < heap->min_ptr->key) { heap->min_ptr = new_node; }

  return new_node;
}

void merge_circular_lists(node_t* left, node_t* right) {
  left->sibling_next->sibling_prev = right->sibling_prev;
  right->sibling_prev->sibling_next = left->sibling_next;
  left->sibling_next = right;
  right->sibling_prev = left;
}

fib_t* heap_union(fib_t* left, fib_t* right) {
  if (!right) { return left; }
  if (!left) { return right; }

  merge_circular_lists(left->min_ptr, right->min_ptr);

  if (right->min_ptr->key < left->min_ptr->key) {
    left->min_ptr = right->min_ptr;
  }
  free(right);

  return left;
}

node_t* cut_out_from_list(node_t* node) {
  if (node == node->sibling_next) { return NULL; }

  node->sibling_next->sibling_prev = node->sibling_prev;
  node->sibling_prev->sibling_next = node->sibling_next;

  node_t* list = node->sibling_next;
  node->sibling_next = node;
  node->sibling_prev = node;

  return list;
}

node_t* heap_link(node_t* parent, node_t* child) {
  cut_out_from_list(child);
  child->parent = parent;
  if (parent->child) {
    merge_circular_lists(child, parent->child);
  } else {
    parent->child = child;
  }
  child->marked = false;
  return parent;
}

#define MAX_DEGREE 16
void consolidate(fib_t* heap) {
  node_t* node_degrees[MAX_DEGREE] = {};

  node_t* node = heap->min_ptr;
  do {
    while (node_degrees[node->degree]) {
      if (node->key < node_degrees[node->degree]->key) {
        node = heap_link(node, node_degrees[node->degree]);
      } else {
        node = heap_link(node_degrees[node->degree], node);
      }
      if (heap->min_ptr->parent) { heap->min_ptr = node; }

      node_degrees[node->degree] = NULL;
      node->degree += 1;
    }
    node_degrees[node->degree] = node;

    node = node->sibling_next;
  } while (node != heap->min_ptr);

  for (int i = 0; i < MAX_DEGREE; ++i) {
    node = node_degrees[i];
    if (node && node->key < heap->min_ptr->key) { heap->min_ptr = node; }
  }
}

KEY peek_min(fib_t* heap) {
  return heap->min_ptr ? heap->min_ptr->key : -1;
}

#define INVALID_KEY -1
KEY extract_min(fib_t* heap) {
  node_t* min = heap->min_ptr;
  if (!min) { return INVALID_KEY; }

  if (min->child) {
    node_t* cursor = min->child;
    do {
      cursor->parent = NULL;
      cursor = cursor->sibling_next;
    } while (cursor != min->child);

    merge_circular_lists(min->child, min);
  }

  if (min == min->sibling_next) {
    heap->min_ptr = NULL;
  } else {
    heap->min_ptr = min->sibling_next;
    cut_out_from_list(min);
    consolidate(heap);
  }
  heap->node_count -= 1;
  KEY min_key = min->key;
  free(min);
  return min_key;
}

void cut(fib_t* heap, node_t* node) {
  node_t* parent = node->parent;
  if (!parent) { return; }
  parent->child = cut_out_from_list(node);
  parent->degree -= 1;
  merge_circular_lists(node, heap->min_ptr);
  node->parent = NULL;
  node->marked = false;

  if (parent->marked) {
    cut(heap, parent);
  } else {
    parent->marked = true;
  }
}

void decrease_key(fib_t* heap, node_t* node, KEY new_key) {
  if (!heap || !node || new_key > node->key) { return; }

  node->key = new_key;
  node_t* parent = node->parent;
  if (parent && new_key < parent->key) { cut(heap, node); }

  if (new_key < heap->min_ptr->key) { heap->min_ptr = node; }
}

void destroy_fib(fib_t* heap) {
  while (extract_min(heap) != INVALID_KEY)
    ;
  free(heap);
}
