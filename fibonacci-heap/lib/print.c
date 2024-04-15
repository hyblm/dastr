#include "../include/fibonnacci-heap-dev.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVERT "\x1b[;7m"
#define DIM "\x1b[;2m"
#define RED "\x1b[;31m"
#define GREEN "\x1b[;32m"
#define BLUE "\x1b[;34m"
#define RESET "\x1b[;0m"

#define OTHER_CHILD "│    "
#define OTHER_ENTRY "├──► "
#define FINAL_ENTRY "╰──► "
#define FINAL_CHILD "     "

#define DEBUG true

void print_key(char *prefix, node_t *node, char *sufix)
{
    printf("%s", prefix);
    if (is_marked(node))
        printf(RED);

    if (node)
    {
        if (DEBUG)
            printf("%d" DIM " %p ←" GREEN " %p " DIM "→ %p" RESET, node->key, node->sibling_prev, node,
                   node->sibling_next);
        else
            printf("%d" RESET, node->key);
    }
    else
        printf("__");
    printf("%s", sufix);
}

#define PREFIX_SEGMENT_LEN 5
char *join(char *prefix, char *end)
{
    unsigned int len = strlen(prefix);
    char *new = (char *)malloc(sizeof(char) * (len + PREFIX_SEGMENT_LEN));
    if (new)
    {
        printf("new: %s, prefix: %s, end: %s\n", new, prefix, end);
        strcpy(new, prefix);
        printf("new: %s, prefix: %s, end: %s\n", new, prefix, end);
        strcpy(new + len, end);
        printf("new: %s, prefix: %s, end: %s\n", new, prefix, end);
    }
    return new;
}

void print_tree(node_t *node, char *prefix)
{
    printf("%s" BLUE, prefix);
    node_t *last = node->sibling_prev;

    while (node != last)
    {
        printf("%s", prefix);
        print_key(OTHER_ENTRY, node, "\n");
        if (node->child)
            print_tree(node->child, join(prefix, OTHER_CHILD));

        node = node->sibling_next;
    }

    print_key(FINAL_ENTRY, node, "\n");
}

void print(fib_t *root)
{
    char *word = root->node_count == 1 ? "node" : "nodes";
    printf(INVERT "▍ %d %s  " RESET "\n", root->node_count, word);
    if (root->min_ptr)
        print_tree(root->min_ptr, "");
    printf("\n");
}
