struct Rectangle {
    width: u32,
    height: u32,
}
impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }
    fn width(&self) -> bool {
        self.width > 0
    }
    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
    fn square(size: u32) -> Self {
        Self {
            width: size,
            height: size,
        }
    }
}

struct Point {
    x: i32,
    y: i32,
}
impl Point {
    fn get_x(&mut self) -> &mut i32 {
        &mut self.x
    }
}

fn main() {
    let width1 = 30;
    let height1 = 50;
    println!(
        "The area of the rectangle is {} square pixels",
        area(width1, height1)
    );
    println!(
        "The area of the rectangle is {} square pixels using area2 functin",
        area2((width1, height1))
    );
    let rectangle1 = Rectangle {
        width: 30,
        height: 20,
    };
    let rectangle2 = Rectangle {
        width: 10,
        height: 10,
    };
    println!("Can rect1 hold rect2 {}", rectangle1.can_hold(&rectangle2));
    println!(
        "The area of the rectangle is {} square pixels using area3 function",
        rectangle1.area()
    );
    if rectangle1.width() {
        println!("The width of the rectangle is greater than 0");
    }
    let mut p = Point { x: 1, y: 2 };
    let x = p.get_x();
    *x += 1;
    println!("{} {}", *x, p.y);
}
fn area(width: u32, height: u32) -> u32 {
    width * height
}
fn area2(dimensions: (u32, u32)) -> u32 {
    dimensions.0 * dimensions.1
}
