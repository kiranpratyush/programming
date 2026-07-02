// use rand::Rng;
// use std::cmp::Ordering;
// use std::io;

fn incr(n: &mut i32) {
    *n += 1;
}

fn main() {
    let mut n = 1;
    incr(&mut n);
    println!("{n}");
}

// fn main() {
//     println!("Guessing game");
//     loop {
//         let secret_number = rand::thread_rng().gen_range(1..=100);
//         println!("Enter your input");
//         let mut guess: String = String::from("");
//         io::stdin()
//             .read_line(&mut guess)
//             .expect("Failed to readline");
//         let guess: u32 = match guess.trim().parse() {
//             Ok(num) => num,
//             Err(_) => continue,
//         };
//         println!("You guessed {guess}");
//         match guess.cmp(&secret_number) {
//             Ordering::Less => println!("Too small"),
//             Ordering::Greater => println!("Too big"),
//             Ordering::Equal => println!("You win"),
//         }
//     }
// }
// fn main() {
//     let mut m1: String = String::from("Hello");
//     let mut m2: String = String::from("World");
//     (m1, m2) = greet(m1, m2);
//     println!("{} {}!", m1, m2);
//     greet2(&m1, &m2);
//     let m3: &String = &String::from("Hello");
//     println!("{}", m3);
// }

// fn greet(g1: String, g2: String) -> (String, String) {
//     println!("{} {}!", g1, g2);
//     (g1, g2)
// }
// fn greet2(g1: &String, g2: &String) {
//     println!("{} {}!", g1, g2);
// }
