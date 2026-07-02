struct Rectangle {
    width: u32,
    height: u32,
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
    println!(
        "The area of the rectangle is {} square pixels using area3 function",
        area3(&rectangle1)
    );
}
fn area(width: u32, height: u32) -> u32 {
    width * height
}
fn area2(dimensions: (u32, u32)) -> u32 {
    dimensions.0 * dimensions.1
}
fn area3(rectangle: &Rectangle) -> u32 {
    rectangle.width * rectangle.height
}
