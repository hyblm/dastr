use binomial_heap::*;

fn main() {
    let mut heap = BinomialHeap::<u32>::new();

    heap.insert(5);
    heap.insert(6);
    heap.insert(7);
    heap.insert(3);
    heap.insert(4);
    heap.insert(1);
    heap.insert(2);
    let node = heap.insert(8);
    heap.insert(9);
    heap.print_tree("Initial");

    let min = heap.extract_min();
    heap.print_tree("Post extraction");
    println!("extracted: {:?}", min);

    heap.decrease_key(node, 1);
    heap.print_tree("Decreased 8 to 1");
}
