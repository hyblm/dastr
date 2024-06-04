#include "splay_tree.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct node {
  KEY key;
  NodeRef le;
  NodeRef gt;
};

NodeRef create_node(KEY value) {
  NodeRef new_node = (NodeRef)malloc(sizeof(struct node));
  if (new_node != NULL) { *new_node = (struct node){value, NULL, NULL}; }
  return new_node;
}

void destroy_node(NodeRef node) {
  free(node);
}

NodeRef pick_subtree(NodeRef root, KEY value) {
  return value <= root->key ? root->le : root->gt;
}

void zig(NodeRef parent, NodeRef child) {
  if (child->key <= parent->key) {
    parent->le = child->gt;
    child->gt = parent;
  } else {
    parent->gt = child->le;
    child->le = parent;
  }
}

void zigzig(NodeRef grandparent, NodeRef parent, NodeRef child) {
  zig(grandparent, parent);
  zig(parent, child);
}

void zigzag(NodeRef grandparent, NodeRef parent, NodeRef child) {
  zig(parent, child);
  zig(grandparent, child);
}

void splay(TreeRef root, KEY value) {
  if (!root) return;
  NodeRef root_node = *root;
  if (!root_node) return;

  if (value == root_node->key) return;

  bool child_is_le = value <= root_node->key;
  TreeRef child = child_is_le ? &root_node->le : &root_node->gt;
  NodeRef child_node = *child;

  if (value == child_node->key) {
    zig(root_node, child_node);
    *root = child_node;
  } else {
    bool grandchild_is_le = value <= child_node->key;
    NodeRef grandchild_node =
        grandchild_is_le ? child_node->le : child_node->gt;

    if (value == grandchild_node->key) {
      bool direction_of_subtrees_matches = child_is_le == grandchild_is_le;
      if (direction_of_subtrees_matches) {
        zigzig(root_node, child_node, grandchild_node);
      } else {
        zigzag(root_node, child_node, grandchild_node);
      }
      *root = grandchild_node;
    } else {
      splay(child, value);
      splay(root, value);
    }
  }
}

void insert_node(TreeRef root, NodeRef node) {
  if (!root) return;
  NodeRef root_node = *root;

  if (!root_node) {
    *root = node;
  } else {
    insert_node(node->key <= root_node->key ? &root_node->le : &root_node->gt,
                node);
  }
}

NodeRef insert(TreeRef root, KEY value) {
  NodeRef new_node = create_node(value);
  if (new_node) {
    insert_node(root, new_node);
    splay(root, value);
  }
  return new_node;
}

void insert_many(TreeRef root, int32_t count, KEY numbers[1]) {
  for (int* end = numbers + count; numbers < end; numbers++) {
    insert(root, *numbers);
  }
}

NodeRef find_node(NodeRef origin, KEY value, NodeRef* last_visited) {
  if (!origin) { return NULL; }

  *last_visited = origin;

  if (value == origin->key) {
    return origin;
  } else {
    return find_node(pick_subtree(origin, value), value, last_visited);
  }
}

NodeRef search(TreeRef root, KEY value) {
  NodeRef last_visited;
  NodeRef node = find_node(*root, value, &last_visited);
  splay(root, last_visited->key);
  return node;
}

NodeRef max(TreeRef tree) {
  NodeRef node = *tree;
  return (node)->gt ? max(&node->gt) : node;
}

void remove_node(TreeRef root, KEY node_key) {
  splay(root, node_key);

  NodeRef root_node = *root;
  NodeRef le = root_node->le;
  NodeRef gt = root_node->gt;
  destroy_node(root_node);

  NodeRef le_max = max(&le);
  *root = le;
  splay(root, le_max->key);

  (*root)->gt = gt;
}

#define RED "\x1b[;31m"
#define BLUE "\x1b[;34m"
#define RESET "\x1b[;0m"

#define NODE ""
#define LE BLUE "│ " RESET
#define GT RED "─╮" RESET
#define BLANK "  "

void print_connection_lines(int inset, int lines) {
  for (int i = inset - 1; i >= 0; i--) {
    printf(((1 << i) & lines) ? LE : BLANK);
  }
}

void print_tree(Tree tree, int inset, int lines) {
  print_connection_lines(inset, lines);
  printf(NODE "%d", tree->key);
  if (tree->gt) { printf(GT); }
  printf("\n");

  int child_inset = inset + 1;
  int child_lines = lines << 1;
  if (tree->le) { child_lines |= 1; }

  if (tree->gt) {
    print_tree(tree->gt, child_inset, child_lines);
  } else if (tree->le) {
    print_connection_lines(child_inset, child_lines);
    printf("\n");
  }

  if (tree->le) { print_tree(tree->le, inset, lines); }
}

void print(TreeRef tree) {
  if (!tree) return;
  Tree root_node = *tree;
  if (!root_node) return;

  print_tree(root_node, 0, 0);
}
