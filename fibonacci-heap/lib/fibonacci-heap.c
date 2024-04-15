#include "../include/fibonnacci-heap-dev.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MARKED_MASK 0xf0;

fib_t *make_fib()
{
    fib_t *tree = (fib_t *)malloc(sizeof(fib_t));
    if (tree)
        *tree = (fib_t){0, NULL};

    return tree;
}

bool is_marked(node_t *node)
{
    return node->degree & MARKED_MASK;
}

void toggle_marked(node_t *node)
{
    node->degree = node->degree ^ MARKED_MASK;
}

node_t *make_node(KEY key, node_t *sibling_prev, node_t *sibling_next)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node)
    {
        if (!sibling_prev)
            sibling_prev = new_node;

        if (!sibling_next)
            sibling_next = sibling_prev;

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

void check_update_min(fib_t *root, node_t *node)
{
    if (!node || !root)
        return;

    if (!root->min_ptr || (node->key < root->min_ptr->key))
        root->min_ptr = node;
}

void update_min(fib_t *root)
{
    node_t *last = root->min_ptr;
    node_t *node = root->min_ptr;
    while (true)
    {
        check_update_min(root, node);

        node = node->sibling_next;
        if (node == last)
            return;
    }
}

int insert(fib_t *root, KEY key)
{
    if (!root->min_ptr)
    {
        root->min_ptr = make_node(key, NULL, NULL);
        if (root->min_ptr)
        {
            root->node_count += 1;
            return 1;
        }
        return -1;
    }

    node_t *new_node = make_node(key, root->min_ptr->sibling_prev, root->min_ptr);
    if (!new_node)
        return -1;

    root->node_count += 1;

    if (key < root->min_ptr->key)
        root->min_ptr = new_node;

    return 1;
}

KEY extract_min(fib_t root)
{
    KEY min_key = root.min_ptr->key;

    node_t *node_degrees[16] = {};

    node_t *dead_ptr = root.min_ptr;
    node_t *node = root.min_ptr;
    while (true)
    {
        uint8_t degree = node->degree;
        node_t *same_degree = node_degrees[degree];
        if (same_degree)
        {
            node_degrees[degree] = NULL;
            // TODO (Matyas): merge nodes
        }
        node_degrees[node->degree] = node;

        node = node->sibling_next;
        if (node == root.min_ptr)
            break;
    }
    free(dead_ptr);

    return min_key;
}

void move_to_min_ptr(fib_t root, node_t *node)
{
    if (node->parent->child == node)
    {
        node->parent->child = node->sibling_next;
    }
    node->sibling_prev->sibling_next = node->sibling_next;
    node->sibling_next->sibling_prev = node->sibling_prev;

    *node = (node_t){.sibling_next = root.min_ptr, .sibling_prev = root.min_ptr->sibling_prev};
    root.min_ptr->sibling_prev->sibling_next = node;
    root.min_ptr->sibling_prev = node;

    if (is_marked(node->parent))
    {
        move_to_min_ptr(root, node->parent);
    }
    toggle_marked(node->parent);
}

void decrease_key(fib_t root, node_t *node, KEY new_key)
{
    node->key = new_key;
    if (new_key < node->parent->key)
    {
        move_to_min_ptr(root, node);
    }
}
