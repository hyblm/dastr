#include "include/fibonnacci-heap.h"
int main(void)
{
    fib_t *queue = make_fib();
    print(queue);

    insert(queue, 7);
    print(queue);

    insert(queue, 3);
    print(queue);

    insert(queue, 5);
    print(queue);

    insert(queue, 8);
    print(queue);

    insert(queue, 2);
    print(queue);

    return 0;
}
