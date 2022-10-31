pub trait FootageSource {
    fn update_texture(&mut self);
}

pub struct AFrameFootage {

}


impl FootageSource for AFrameFootage {
    fn update_texture(&mut self) {
        todo!()
    }
}

pub struct AFootage {
    source: Box<dyn FootageSource>,
}