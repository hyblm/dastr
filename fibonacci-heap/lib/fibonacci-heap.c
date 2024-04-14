#include <stdint.h>

struct FibonacciTree {
    node_t* root;
    node_t* min_ptr;
}

struct Node {
    int32_t key;
    node_t* parent;
    node_t* sibling;
    node_t* child;
};
