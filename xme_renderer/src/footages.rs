use crate::{renderer::Renderer, texture::Texture};

pub trait FootageSource {
    fn render(&mut self, renderer: &mut Renderer) -> &Texture;
}

pub struct AFrameFootage {
    texture: Option<Texture>,
    pixels: Vec<u8>,
    width: u32,
    height: u32,
}
unsafe impl Send for AFrameFootage {}
unsafe impl Sync for AFrameFootage {}

impl FootageSource for AFrameFootage {
    fn render(&mut self, renderer: &mut Renderer) -> &Texture {
        if self.texture.is_none() {
            self.texture = Some(
                Texture::from_rgba(
                    &renderer.device,
                    &renderer.queue,
                    &self.pixels,
                    (self.width, self.height),
                    None,
                )
                .unwrap(),
            );
        }
        return &self.texture.as_ref().unwrap();
    }
}

impl AFrameFootage {
    pub fn new() -> Self {
        Self {
            texture: None,
            pixels: vec![],
            width: 0,
            height: 0,
        }
    }

    pub fn set_pixels(&mut self, pixels: Vec<u8>, width: u32, height: u32) {
        self.pixels = pixels;
        self.width = width;
        self.height = height;
    }
}
