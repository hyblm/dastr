#include <stdlib.h>

#define KEY int32_t
#define INVALID_KEY -1

typedef struct node *NodeRef;
typedef NodeRef Tree;
typedef Tree *TreeRef;

NodeRef insert(TreeRef root, KEY value);
void insert_many(TreeRef root, int32_t count, KEY numbers[1]);

void remove_node(TreeRef root, KEY node_key);

NodeRef search(TreeRef root, KEY value);

void print(TreeRef tree);
