use crate::footages::{self, FootageSource};

pub struct ALayer {
    id: i64,
    footage: Box<dyn FootageSource>,
}