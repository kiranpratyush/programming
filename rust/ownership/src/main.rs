fn first_word(s: &String) -> usize {
    let bytes = s.as_bytes();
    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return i;
        }
    }
    s.len()
}

fn main() {
    let mut s = String::from("hello");
    for &item in s.as_bytes().iter() {
        if item == b'l' {}
    }
    s.push_str(" world");
}
