#include <stdlib.h>

#define KEY int
#define INVALID_KEY -1

typedef struct node *NodeRef;
typedef NodeRef Splay;
typedef Splay *SplayRef;

/// left <= value,
/// right > value
struct node {
  KEY key;
  struct node *left;
  struct node *right;
};

struct node *create_node(KEY value);

/// Returns a pointer to the newly inserted node
/// for use in future operations (eg. remove)
struct node *insert(SplayRef root, KEY value);

struct node *search(SplayRef root, KEY value);

void remove_node(SplayRef root, KEY node_key);
void print(SplayRef tree);
