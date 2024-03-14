use std::{cell::RefCell, fmt::Debug, fmt::Display, rc::Rc};

#[derive(Clone)]
pub struct Node<T: Ord + Clone> {
    pub key: T,
    pub degree: usize,
    pub parent: Option<WrappedNode<T>>,
    pub sibling: Option<WrappedNode<T>>,
    pub child: Option<WrappedNode<T>>,
}
pub type WrappedNode<T> = Rc<RefCell<Node<T>>>;

impl<T: Ord + Clone> Node<T> {
    fn new(value: T) -> Self {
        Self {
            key: value,
            degree: 0,
            parent: None,
            sibling: None,
            child: None,
        }
    }

    pub fn wrapped(value: T) -> WrappedNode<T> {
        Rc::new(RefCell::new(Node::new(value)))
    }

    pub fn wrap(self) -> WrappedNode<T> {
        Rc::new(RefCell::new(self))
    }

    pub fn link(left: WrappedNode<T>, right: WrappedNode<T>) {
        let mut left_tree = (*left).borrow_mut();
        let mut right_tree = (*right).borrow_mut();

        right_tree.parent = Some(left.clone());
        right_tree.sibling = left_tree.child.replace(right.clone());
        left_tree.degree += 1;
    }

    pub fn sibling_count(&self) -> usize {
        self.siblings().count()
    }

    pub fn siblings(&self) -> Siblings<T> {
        Siblings {
            inner: Some(self.clone().wrap()),
        }
    }
}
#[derive(Clone)]
pub struct Siblings<T: Ord + Clone> {
    pub inner: Option<WrappedNode<T>>,
}

impl<T: Ord + Clone> Iterator for Siblings<T> {
    type Item = WrappedNode<T>;

    fn next(&mut self) -> Option<Self::Item> {
        let x = self.inner.take();
        if let Some(ref node) = x {
            self.inner = (**node).borrow().sibling.clone();
        };
        x
    }
}

impl<T: Ord + Clone> Eq for Node<T> {}
impl<T: Ord + Clone> PartialEq for Node<T> {
    fn eq(&self, other: &Self) -> bool {
        self.degree == other.degree
    }
}
impl<T: Ord + Clone> PartialOrd for Node<T> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(Ord::cmp(self, other))
    }
}
impl<T: Ord + Clone> Ord for Node<T> {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.degree.cmp(&other.degree)
    }
}

impl<T: Ord + Clone + Display> Display for Node<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.key)
    }
}

impl<T: Ord + Clone + Debug> Debug for Node<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let Self {
            key,
            degree,
            parent,
            sibling,
            child,
        } = self;
        let mut node_string = String::new();
        node_string.push_str(&format!("({key:?} :: {degree})",));
        if let Some(node) = parent {
            node_string.push_str(&format!("[parent: {:?}]", (*node).borrow().key));
        };
        if let Some(node) = sibling {
            node_string.push_str(&format!("[sibling: {:?}]", (*node).borrow().key));
        };
        if let Some(node) = child {
            node_string.push_str(&format!("[child: {:?}]", (*node).borrow().key));
        };
        write!(f, "{node_string}")
    }
}
