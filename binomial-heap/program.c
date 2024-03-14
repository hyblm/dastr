#include "binomial-heap.h"
#include <stdio.h>

int32_t main(void)
{
    BinomialHeap_t heap = binomial_heap_create(8);
    node_print(heap);

    BinomialHeap_t heap2 = binomial_heap_create(12);
    node_print(heap2);

    heap = binomial_tree_same_degree_merge(&heap, &heap2);
    node_print(heap);
}
