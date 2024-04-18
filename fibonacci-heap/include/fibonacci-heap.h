#include <stdint.h>

#define KEY int32_t

typedef struct Node node_t;
typedef struct FibonacciTree
{
    uint32_t node_count;
    node_t *min_ptr;
} fib_t;

fib_t *make_fib();
void destroy_fib(fib_t* heap);

void print(fib_t* root);

node_t* insert(fib_t* root, KEY key);

KEY peek_min(fib_t* root);
KEY extract_min(fib_t* root);

void decrease_key(fib_t* root, node_t* node, KEY new_key);
