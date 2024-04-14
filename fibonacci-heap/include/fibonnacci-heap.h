typedef struct FibonacciTree fib_t;
typedef struct Node node_t;

void print(fib_t* root);

int insert(fib_t* root, int32_t key);

int32_t peek_min(fib_t* root);
int32_t extract_min(fib_t* root);

int32_t decrease_key(node_t* node, int32_t new_key);
