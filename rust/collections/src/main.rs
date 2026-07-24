use std::collections::{HashMap, hash_map};
fn main() {
    let mut scores: HashMap<String, i32> = HashMap::new();
    scores.insert(String::from("Blue"), 1);
    scores.insert(String::from("Yello"), 50);
    let yellow = scores.entry(String::from("Yellow")).or_insert(50);
}
