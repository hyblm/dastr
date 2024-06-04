#[derive(Clone, Debug)]
pub struct VanEmdeBoas {
    universe_size: usize,
    bounds: Option<(usize, usize)>,
    summary: Option<Box<VanEmdeBoas>>,
    clusters: Vec<VanEmdeBoas>,
}

const DIM: &str = "\x1b[;2m";
const RED: &str = "\x1b[;31m";
const GREEN: &str = "\x1b[;32m";
const RESET: &str = "\x1b[;0m";

const fn high_root(value: usize) -> usize {
    2_usize.pow(value.ilog2().div_ceil(2))
}

const fn low_root(value: usize) -> usize {
    2_usize.pow(value.ilog2() / 2)
}

impl VanEmdeBoas {
    const fn partition_value(&self, value: usize) -> (usize, usize) {
        let low_root = low_root(self.universe_size);
        (value / low_root, value % low_root)
    }

    const fn index(&self, y: usize, z: usize) -> usize {
        (y * low_root(self.universe_size)) + z
    }

    pub fn new(size: usize) -> Self {
        let universe_size = size.next_power_of_two();

        let mut clusters = vec![];
        let summary = if universe_size > 2 {
            let summary_size = high_root(universe_size);
            for _ in 0..summary_size {
                clusters.push(VanEmdeBoas::new(low_root(universe_size)));
            }
            Some(Box::new(VanEmdeBoas::new(summary_size)))
        } else {
            None
        };

        Self {
            universe_size,
            bounds: None,
            summary,
            clusters,
        }
    }

    pub const fn is_empty(&self) -> bool {
        self.bounds.is_none()
    }

    pub fn min(&self) -> Option<usize> {
        self.bounds.map(|(min, _)| min)
    }

    pub fn max(&self) -> Option<usize> {
        self.bounds.map(|(_, max)| max)
    }

    pub fn insert(&mut self, value: usize) {
        if value >= self.universe_size {
            eprintln!(
                "value {} doesn't fit in universe size {}",
                value, self.universe_size
            );
            return;
        }
        self.insert_unchecked(value);
    }

    pub fn insert_unchecked(&mut self, mut value: usize) {
        let Some((min, max)) = self.bounds.as_mut() else {
            self.bounds = Some((value, value));
            return;
        };

        if value > *max {
            *max = value;
        }

        if value < *min {
            std::mem::swap(min, &mut value);
        }

        if self.summary.is_some() {
            let (high, low) = self.partition_value(value);

            if self.clusters[high].is_empty() {
                if let Some(summary) = self.summary.as_mut() {
                    summary.insert(high);
                };
            };
            self.clusters[high].insert(low);
        }
    }

    pub fn contains(&self, value: usize) -> bool {
        value < self.universe_size && self.contains_unchecked(value)
    }

    pub fn contains_unchecked(&self, value: usize) -> bool {
        match self.bounds {
            Some((min, max)) if (value == min || value == max) => true,
            _ => {
                if self.universe_size == 2 {
                    false
                } else {
                    let (high, low) = self.partition_value(value);
                    self.clusters[high].contains(low)
                }
            }
        }
    }

    pub fn successor(&self, value: usize) -> Option<usize> {
        let Some((min, max)) = self.bounds else {
            return None;
        };

        if self.universe_size == 2 {
            return (value < max).then(|| max);
        };

        let successor = if value < min {
            min
        } else {
            let (high, low) = self.partition_value(value);

            let (y, z) = if self.clusters[high].max().is_some_and(|max| low < max) {
                (high, self.clusters[high].successor(low)?)
            } else {
                let next_cluster = self.summary.as_ref()?.successor(high)?;
                (next_cluster, self.clusters[next_cluster].min()?)
            };

            self.index(y, z)
        };
        Some(successor)
    }

    pub fn print(&self) {
        self.print_tree("   ");
    }

    fn print_tree(&self, prefix: &str) {
        let Some((min, max)) = self.bounds else {
            println!();
            return;
        };
        print!("{DIM}─►{RESET} ");
        println!(
            "{RED}{}⮯ {GREEN}{}⮭ {DIM}({}){RESET}",
            min, max, self.universe_size
        );

        let count = self.clusters.len();
        if let Some(ref summary) = self.summary {
            summary.print_sub(prefix, "su", count > 0);
        }
        for (i, cluster) in self.clusters.iter().enumerate() {
            cluster.print_sub(prefix, &format!("{:02b}", i), i < count - 1);
        }
    }

    fn print_sub(&self, prefix: &str, tag: &str, is_last: bool) {
        let (b, a) = if is_last {
            ("│      ", "├─")
        } else {
            ("       ", "╰─")
        };
        print!("{}{}{}{}{}", DIM, prefix, a, RESET, tag);
        self.print_tree(&format!("{}{}", prefix, b));
    }

    pub fn iter(&self) -> VanEmdeBoasWalker<'_> {
        VanEmdeBoasWalker {
            tree: self,
            current: self.min(),
        }
    }
}

#[derive(Debug)]
pub struct VanEmdeBoasWalker<'v> {
    tree: &'v VanEmdeBoas,
    current: Option<usize>,
}

impl<'v> Iterator for VanEmdeBoasWalker<'v> {
    type Item = usize;

    fn next(&mut self) -> Option<Self::Item> {
        let Some(next) = self.current.map(|index| self.tree.successor(index))? else {
            return self.current.take();
        };
        self.current.replace(next)
    }
}

#[cfg(test)]
mod tests {
    use crate::VanEmdeBoas;

    #[test]
    fn successor_same_cluster() {
        let nums = vec![1, 2];
        let mut tree = VanEmdeBoas::new(8);
        nums.iter().for_each(|&num| tree.insert(num));
        assert_eq!(nums, tree.iter().collect::<Vec<usize>>())
    }

    #[test]
    fn successor_different_cluster() {
        let nums = vec![1, 4];
        let mut tree = VanEmdeBoas::new(8);
        nums.iter().for_each(|&num| tree.insert(num));
        assert_eq!(nums, tree.iter().collect::<Vec<usize>>())
    }

    #[test]
    fn successor_weird() {
        let nums = vec![1, 6, 7, 8, 9, 15];
        let mut tree = VanEmdeBoas::new(16);
        nums.iter().for_each(|&num| tree.insert(num));
        assert_eq!(nums, tree.iter().collect::<Vec<usize>>())
    }
}
