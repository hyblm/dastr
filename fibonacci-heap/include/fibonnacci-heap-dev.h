#include "fibonnacci-heap.h"
#include <stdbool.h>

struct Node
{
    KEY key;
    node_t *parent;
    node_t *sibling_prev;
    node_t *sibling_next;
    node_t *child;
    uint8_t degree;
};

bool is_marked(node_t *node);
void toggle_marked(node_t *node);
