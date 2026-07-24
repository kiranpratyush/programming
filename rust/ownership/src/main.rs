/*What is the difference between String and string (This is slice)
 *
 *
 *
 */

fn main() {
    let mut s1 = String::from("foo");
    let s2 = "bar";
    s1.push_str(s2);
    println!("s2 is {s2}");
    s1.push('l');
    let s3 = s1 + s2;
    println!("s2 is {}", s2);
    println!("s2 is {}", s3);
}
