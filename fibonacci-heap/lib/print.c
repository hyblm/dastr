#include "../include/fibonacci-heap-dev.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVERT "\x1b[;7m"
#define DIM "\x1b[;2m"
#define RED "\x1b[;31m"
#define GREEN "\x1b[;32m"
#define BLUE "\x1b[;34m"
#define PINK "\x1b[;35m"
#define TEAL "\x1b[;36m"
#define RESET "\x1b[;0m"

#define OTHER_CHILD "│    "
#define OTHER_ENTRY "├──► "
#define FINAL_ENTRY "╰──► "
#define FINAL_CHILD "     "

#define DEBUG false

void print_key(char *prefix, node_t *node, char *sufix)
{
    printf("%s", prefix);
    if (node->marked)
    {
        printf(RED);
    }

    if (node)
    {
        if (DEBUG)
        {
            printf("%d (%d)" DIM " %p 🢀" GREEN " %p " DIM "🢂 %p" PINK " 🢁 %p" TEAL " 🢃 %p" RESET, node->key,
                   node->degree, node->sibling_prev, node, node->sibling_next, node->parent, node->child);
        }
        else
        {
            printf("%d (%d)" RESET, node->key, node->degree);
        }
    }
    else
    {
        printf("__");
    }
    printf("%s", sufix);
}

char *join(char *prefix, char *end, bool *allocated)
{
    char *new;
    if (!*prefix)
    {
        return end;
    }
    unsigned int midpoint = strlen(prefix);
    unsigned int len = midpoint + strlen(end) + 1;
    new = (char *)malloc(sizeof(char) * len);
    if (new)
    {
        strcpy(new, prefix);
        if (end)
        {
            strcpy(new + midpoint, end);
        }
    }
    *allocated = true;
    return new;
}

void print_tree(node_t *node, char *prefix, bool allocated)
{

    node_t *last = node->sibling_prev;
    while (node != last)
    {
        printf("%s", prefix);
        print_key(OTHER_ENTRY, node, "\n");
        if (node->child)
        {
            bool allocation = false;
            char *child_prefix = join(prefix, OTHER_CHILD, &allocation);
            print_tree(node->child, child_prefix, allocation);
        }

        node = node->sibling_next;
    }

    printf("%s", prefix);
    print_key(FINAL_ENTRY, node, "\n");
    if (node->child)
    {
        bool allocation = false;
        char *child_prefix = join(prefix, FINAL_CHILD, &allocation);
        print_tree(node->child, child_prefix, allocation);
    }
    // printf("prefix: %p \'%s\'\n", prefix, prefix);
    if (allocated)
    {
        free(prefix);
    }
}

void print(fib_t *root)
{
    char *word = root->node_count == 1 ? "node" : "nodes";
    printf(INVERT "▍ %d %s  " RESET "\n", root->node_count, word);
    if (root->min_ptr)
    {
        printf(BLUE);
        print_tree(root->min_ptr, "", false);
    }
    printf("\n");
}
