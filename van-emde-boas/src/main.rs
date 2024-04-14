use van_emde_boas::*;

fn main() {
    let mut tree = VanEmdeBoas::new(32);
    let nums = vec![0, 3, 12, 18, 25, 1, 15];
    nums.iter().for_each(|&num| tree.insert(num));
    tree.print();

    println!();
    println!("{:?}", nums);
    println!("{:?}", tree.iter().collect::<Vec<usize>>());
}
