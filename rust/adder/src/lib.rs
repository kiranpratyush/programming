pub fn add(left: u64, right: u64) -> u64 {
    left + right
}
pub fn add_two(a: i32) -> i32 {
    a + 2
}

#[derive(Debug)]
pub struct Rectangle {
    width: i32,
    height: i32,
}

impl Rectangle {
    pub fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn exploration() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
    #[test]
    fn make_fail() {
        panic!("Making this test fail");
    }
    #[test]
    fn larger_can_hold_smaller() {
        let smaller_rectangle = Rectangle {
            height: 10,
            width: 5,
        };
        let larger_rectangle = Rectangle {
            height: 20,
            width: 10,
        };
        assert!(larger_rectangle.can_hold(&smaller_rectangle));
    }
    #[test]
    fn smaller_can_not_hold_larger() {
        let smaller_rectangle = Rectangle {
            height: 10,
            width: 5,
        };
        let larger_rectangle = Rectangle {
            height: 20,
            width: 10,
        };
        assert!(!smaller_rectangle.can_hold(&larger_rectangle));
    }
    #[test]
    fn it_add_two() {
        let result = add_two(3);
        assert_eq!(result, 5);
    }
}
