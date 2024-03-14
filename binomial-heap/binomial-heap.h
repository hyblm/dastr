#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdint.h>

typedef struct Node Node_t;

Node_t *node_create(int32_t value);
void node_destroy(Node_t *node);
void node_print(Node_t *node);

typedef Node_t *BinomialHeap_t;

BinomialHeap_t binomial_heap_create(int32_t value);
void binomial_heap_destroy(BinomialHeap_t binomial_heap);

/// returns pointer to new root, makes input pointers NULL to prevent reuse
Node_t *binomial_tree_same_degree_merge(Node_t **left, Node_t **right);
Node_t *binomial_heap_union(Node_t **left, Node_t **right);

Node_t *binomial_heap_insert(BinomialHeap_t heap, int32_t);
void binomial_heap_decrease_key(BinomialHeap_t heap, Node_t *node);
int32_t binomial_heap_peek_min(BinomialHeap_t heap);
int32_t binomial_heap_extract_min(BinomialHeap_t heap);

typedef BinomialHeap_t priority_queue_t;
/// returns pointer to newly inserted node
Node_t *pq_enqueue(priority_queue_t, int32_t);
int32_t pq_dequeue(priority_queue_t);

#endif
