/*
Trait : Defines behavior of a type (defines methods, the type should provide if it implements
a trait).
pub trait TraitName{
pub functName();
}
To implement a trait either the trait or the type should be local to the trait.


*/
pub trait Summary {
    fn summarize(&self) -> String;
}
pub struct NewsPaper {
    pub headline: String,
    pub location: String,
    pub author: String,
    pub content: String,
}
pub struct SocialPost {
    pub username: String,
    pub content: String,
    pub reply: bool,
    pub repost: bool,
}
impl Summary for NewsPaper {
    fn summarize(&self) -> String {
        format!("{}, by {} ({})", self.headline, self.author, self.location)
    }
}
impl Summary for SocialPost {
    fn summarize(&self) -> String {
        format!("{}: {}", self.username, self.content)
    }
}

fn largest_i32(list: &[i32]) -> &i32 {
    let mut largest = &list[0];
    for item in list {
        if item > largest {
            largest = item;
        }
    }
    largest
}

fn main() {
    let number_list = vec![34, 50, 25, 100, 65];
    let result = largest_i32(&number_list);
    println!("The largest number is {result}");
    let char_list = vec!['y', 'm', 'a', 'q'];
    println!("The largest char is {result}");
}
