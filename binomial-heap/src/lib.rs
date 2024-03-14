use std::{
    cell::RefCell,
    fmt::Display,
    rc::{Rc, Weak},
};

mod node;
use node::{Node, Siblings, WrappedNode};

#[derive(Clone)]
pub struct BinomialHeap<T: Ord + Clone> {
    root_list: Option<WrappedNode<T>>,
}

impl<T: Ord + Clone> BinomialHeap<T> {
    pub fn new() -> Self {
        Self { root_list: None }
    }

    pub fn union(&mut self, other: Self) {
        self.merge(other);

        let mut last: Option<WrappedNode<T>> = None;
        let mut list = self.roots();
        let Some(mut prev) = list.next() else { return };

        for current in list {
            if (*prev).borrow().degree != (*current).borrow().degree || {
                (*current)
                    .borrow()
                    .sibling
                    .as_ref()
                    .map(|s: &WrappedNode<T>| (**s).borrow().degree == (*current).borrow().degree)
                    .unwrap_or(false)
            } {
                last = Some(prev.clone());
                prev = current.clone();
            } else if (*prev).borrow().key <= (*current).borrow().key {
                (*prev).borrow_mut().sibling = (*current).borrow().sibling.clone();
                Node::link(prev.clone(), current.clone());
            } else {
                if let Some(ref l) = last {
                    (**l).borrow_mut().sibling = Some(current.clone());
                } else {
                    self.root_list = Some(current.clone());
                }
                Node::link(current.clone(), prev.clone());
                prev = current;
            }
        }
    }

    pub fn insert(&mut self, value: T) -> Weak<RefCell<Node<T>>> {
        let node = Node::wrapped(value);
        self.union(Self {
            root_list: Some(node.clone()),
        });
        Rc::downgrade(&node)
    }

    fn reverse(&mut self) {
        let mut last = None;
        let mut cursor = self.root_list.take();

        while let Some(node) = cursor {
            (*node).borrow_mut().parent.take();
            cursor = {
                let sibling = &mut (*node).borrow_mut().sibling;
                match last {
                    Some(previous) => sibling.replace(previous),
                    None => sibling.take(),
                }
            };
            last = Some(node);
        }
        self.root_list = last;
    }

    pub fn extract_min(&mut self) -> Option<T> {
        let min = self.extract_min_root()?;

        if let Some(child) = (*min).borrow().clone().child {
            let mut orphaned = Self {
                root_list: Some(child),
            };
            orphaned.reverse();
            self.union(orphaned)
        }

        Rc::into_inner(min).map(|out| out.into_inner().key)
    }

    pub fn extract_min_root(&mut self) -> Option<WrappedNode<T>> {
        let mut prev = self.root_list.clone();
        let mut before_min = None;
        let mut min = prev.clone()?;

        for current in self.roots() {
            if (*current).borrow().key < (*min).borrow().key {
                min = current.clone();
                before_min = prev.clone();
            };
            prev.replace(current);
        }

        let x = (*min).borrow_mut().sibling.take();
        if let Some(before) = before_min.clone() {
            (*before).borrow_mut().sibling = x;
        } else {
            self.root_list = x;
        }

        Some(min)
    }

    pub fn decrease_key(&self, node: Weak<RefCell<Node<T>>>, new_value: T) {
        let Some(node) = node.upgrade() else { return };
        assert!((*node).borrow().key > new_value);

        (*node).borrow_mut().key = new_value;
        let mut cursor = node.clone();
        let mut list = (*cursor).borrow().parent.clone();
        while let Some(ref mut parent) = list {
            {
                let mut c = (*cursor).borrow_mut();
                let mut p = (**parent).borrow_mut();
                if p.key < c.key {
                    break;
                };
                let temp = p.key.clone();
                p.key = c.key.clone();
                c.key = temp;
            }

            cursor = parent.clone();
            list = (*cursor).borrow().parent.clone();
        }
    }

    fn merge(&mut self, other: Self) {
        let mut head: Option<WrappedNode<T>> = None;
        let mut tail = head.clone();

        let mut left = self.roots().peekable();
        let mut right = other.roots().peekable();

        let mut next_smallest = || -> Option<WrappedNode<T>> {
            if left.peek() < right.peek() {
                left.next()
            } else {
                right.next()
            }
            .or_else(|| left.next().xor(right.next()))
        };

        while let Some(x) = next_smallest() {
            if let Some(node) = tail {
                (*node).borrow_mut().sibling = Some(x.clone());
            }
            tail = Some(x);
            if head.is_none() {
                head = tail.clone();
            }
        }

        self.root_list = head;
    }

    fn roots(&self) -> Siblings<T> {
        node::Siblings {
            inner: self.root_list.clone(),
        }
    }
}

impl<T: Ord + Clone + Display> BinomialHeap<T> {
    pub fn print_tree(&self, name: &str) {
        const OTHER_CHILD: &str = "│    ";
        const OTHER_ENTRY: &str = "├──► ";
        const FINAL_ENTRY: &str = "╰──► ";
        const FINAL_CHILD: &str = "     ";

        println!("\n{}", name);
        if let Some(l) = self.root_list.clone() {
            visit(l, "")
        };
        fn visit<T: Ord + Clone + Display>(root: WrappedNode<T>, prefix: &str) {
            let mut count = (*root).borrow().clone().sibling_count();

            for node in (*root).borrow().siblings() {
                count -= 1;
                let connector = if count == 0 { FINAL_ENTRY } else { OTHER_ENTRY };
                let inner = (*node).borrow();

                if let Some(child) = inner.child.clone() {
                    println!("{}{}{}", prefix, connector, inner);
                    let addon = if count == 0 { FINAL_CHILD } else { OTHER_CHILD };
                    let new_prefix = format!("{}{}", prefix, addon);
                    visit(child, &new_prefix);
                } else {
                    println!("{}{}{}", prefix, connector, inner);
                }
            }
        }
    }
}

impl<T: Ord + Clone> Default for BinomialHeap<T> {
    fn default() -> Self {
        Self::new()
    }
}
