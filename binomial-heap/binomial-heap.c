#include "binomial-heap.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int32_t key;
    size_t degree;

    Node_t *parent;
    Node_t *sibling;
    Node_t *child;
};

Node_t *node_create(int32_t value)
{
    Node_t *node = (Node_t *)malloc(sizeof(Node_t));
    if (node)
    {
        *node = (Node_t){.key = value};
    }
    return node;
}

BinomialHeap_t binomial_heap_create(int32_t value)
{
    return node_create(value);
}

Node_t *binomial_tree_same_degree_merge(Node_t **left_ptr, Node_t **right_ptr)
{
    Node_t *left = *left_ptr;
    Node_t *right = *right_ptr;
    assert(left->degree == right->degree);

    *left_ptr = NULL;
    *right_ptr = NULL;

    Node_t *root, *other;
    if (left->key < right->key)
    {
        root = left;
        other = right;
    }
    else
    {
        root = right;
        other = left;
    }

    root->sibling = other->sibling;
    other->sibling = root->child;

    other->parent = root;
    root->child = other;

    root->degree += 1;

    return root;
}

Node_t *binomial_heap_merge(Node_t **left_ptr, Node_t **right_ptr)
{
    Node_t *left = *left_ptr;
    Node_t *right = *right_ptr;
    *left_ptr = NULL;
    *right_ptr = NULL;

    if (!left)
        return right;
    if (!right)
        return left;

    Node_t *head;
    if (left->degree < right->degree)
    {
        head = left;
        left = left->sibling;
    }
    else
    {
        head = right;
        right = right->sibling;
    }

    Node_t *tail = head;

    while (left && right)
    {
        if (left->degree < right->degree)
        {
            tail->sibling = left;
            left = left->sibling;
        }
        else
        {
            tail->sibling = right;
            right = right->sibling;
        }
        tail = tail->sibling;
    }

    tail->sibling = left ? left : right;

    return head;
}

Node_t *binomial_heap_union(Node_t **left_ptr, Node_t **right_ptr)
{
    BinomialHeap_t heap = binomial_heap_merge(left_ptr, right_ptr);
    if (heap == NULL)
        return NULL;

    Node_t *last = NULL;
    Node_t *this = heap;
    Node_t *next = heap->sibling;

    while (next)
    {
        const int32_t move_pointers =
            (this->degree != next->degree) || (next->sibling && next->sibling->degree == next->degree);

        if (move_pointers)
        {
            last = this;
            this = next;
        }
        else
        {
            // TODO (matyas): merge trees of matching degree
        }
        next = next->sibling;
    }

    return heap;
}

BinomialHeap_t binomial_heap_insert(BinomialHeap_t heap, int32_t value)
{
    BinomialHeap_t new_heap = binomial_heap_create(value);
    return binomial_heap_union(&heap, &new_heap);
}

int32_t binomial_heap_extract_min(BinomialHeap_t heap)
{
    Node_t *min = heap;
    while (heap)
    {
        if (heap->key < min->key)
            min = heap;
        heap = heap->sibling;
    }

    // remove the node

    return min->key;
}

int32_t binomial_heap_peek_min(BinomialHeap_t heap)
{
    int32_t min = heap->key;
    while (heap)
    {
        if (heap->key < min)
            min = heap->key;
        heap = heap->sibling;
    }
    return min;
}

void binomial_heap_decrease_key(BinomialHeap_t heap, Node_t *node);

void node_print(Node_t *node)
{
    printf("(%d) { deg: %ld child: %d parent: %d sibling: %d }\n", node->key, node->degree,
           node->child ? node->child->key : 0, node->parent ? node->parent->key : 0,
           node->sibling ? node->sibling->key : 0);
}
