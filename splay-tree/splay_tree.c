#include "splay_tree.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

NodeRef create_node(KEY value)
{
  NodeRef new_node = (NodeRef)malloc(sizeof(struct node));
  if (new_node != NULL)
    {
      *new_node = (struct node){value, NULL, NULL};
    }
  return new_node;
}

void destroy_node(NodeRef node) { free(node); }

NodeRef pick_subtree(NodeRef root, KEY value)
{
  return value <= root->key ? root->left : root->right;
}

void zig(NodeRef parent, NodeRef child)
{
  if (child->key <= parent->key)
    {
      parent->left = child->right;
      child->right = parent;
    }
  else
    {
      parent->right = child->left;
      child->left = parent;
    }
}

void zigzig(NodeRef grandparent, NodeRef parent, NodeRef child)
{
  zig(grandparent, parent);
  zig(parent, child);
}

void zigzag(NodeRef grandparent, NodeRef parent, NodeRef child)
{
  zig(parent, child);
  zig(grandparent, child);
}

void splay(SplayRef root, KEY value)
{
  if (!root) return;
  NodeRef root_node = *root;
  if (!root_node) return;

  if (value == root_node->key) return;

  bool is_left = value <= root_node->key;
  SplayRef sub_tree = is_left ? &root_node->left : &root_node->right;
  NodeRef sub_tree_node = *sub_tree;

  if (value == sub_tree_node->key)
    {
      zig(root_node, sub_tree_node);
      *root = sub_tree_node;
    }
  else
    {
      bool is_left_2 = value <= sub_tree_node->key;
      NodeRef sub_sub_tree_node =
          is_left_2 ? sub_tree_node->left : sub_tree_node->right;

      if (value == sub_sub_tree_node->key)
        {
          if (is_left == is_left_2)
            {
              zigzig(root_node, sub_tree_node, sub_sub_tree_node);
            }
          else
            {
              zigzag(root_node, sub_tree_node, sub_sub_tree_node);
            }
          *root = sub_sub_tree_node;
        }
      else
        {
          splay(sub_tree, value);
          splay(root, value);
        }
    }
}

void insert_node(SplayRef root, NodeRef node)
{
  if (!root) return;
  NodeRef root_node = *root;

  if (!root_node)
    {
      *root = node;
    }
  else
    {
      insert_node(
          node->key <= root_node->key ? &root_node->left : &root_node->right,
          node);
    }
}

NodeRef insert(SplayRef root, KEY value)
{
  NodeRef new_node = create_node(value);
  if (new_node)
    {
      insert_node(root, new_node);
      splay(root, value);
    }
  return new_node;
}

NodeRef find_node(NodeRef origin, KEY value, NodeRef* last_visited)
{
  if (!origin)
    {
      return NULL;
    }
  *last_visited = origin;

  if (value == origin->key)
    {
      return origin;
    }
  else
    {
      return find_node(pick_subtree(origin, value), value, last_visited);
    }
}

NodeRef search(SplayRef root, KEY value)
{
  NodeRef last_visited;
  NodeRef node = find_node(*root, value, &last_visited);
  splay(root, last_visited->key);
  return node;
}

NodeRef max(NodeRef node) { return node->right ? max(node->right) : node; }

void remove_node(SplayRef root, KEY node_key)
{
  // NOTE (osicka): 1. splay
  splay(root, node_key);

  // NOTE (osicka): 2. odebereme koren, zustanou nam 2 podstromy
  NodeRef root_node = *root;
  NodeRef left = root_node->left;
  NodeRef right = root_node->right;
  destroy_node(root_node);

  // NOTE (osicka): 3. najdeme max leveho podstromu a zavolame na nej splay
  // maximum leveho podstromu nikdy nebude mit praveho potomka (>)
  NodeRef left_max = max(left);
  *root = left;
  splay(root, left_max->key);

  // NOTE (osicka): 4. jako praveho potomka y pripojime pravy podstrom
  (*root)->right = right;
}

#define MAX_SHIFT sizeof(int) * 8

#define INVERT "\x1b[;7m"
#define DIM "\x1b[;2m"
#define RED "\x1b[;31m"
#define GREEN "\x1b[;32m"
#define BLUE "\x1b[;34m"
#define PINK "\x1b[;35m"
#define TEAL "\x1b[;36m"
#define RESET "\x1b[;0m"

#define NODE ""
#define LEFT BLUE "│ " RESET
// #define RIGHT RED "╰─" RESET
#define RIGHT RED "─╮" RESET
#define BLANK "   "

void print_tree(NodeRef tree, int inset, int lines)
{
  // printf("( %d %b ) ", inset, lines);
  for (int i = inset - 1; i >= 0; i--)
    {
      printf(((1 << i) & lines) ? LEFT : BLANK);
    }
  if (!tree) return;
  printf(NODE "%d", tree->key);

  int child_inset = inset + 1;
  int child_lines = lines << 1;
  if (tree->left)
    {
      child_lines = child_lines | 1;
    }

  if (tree->right)
    {
      puts(RIGHT);
      print_tree(tree->right, child_inset, child_lines);
    }
  else if (tree->left)
    {
      puts("");
      print_tree(NULL, child_inset, child_lines);
      puts("");
    }
  else
    {
      puts("");
    }

  if (tree->left) print_tree(tree->left, inset, lines);
}

void print(SplayRef tree)
{
  // printf("%lu %0b\n", MAX_SHIFT, INT_MAX);
  if (!tree) return;
  NodeRef root_node = *tree;
  if (!root_node) return;

  print_tree(root_node, 0, 0);
}
