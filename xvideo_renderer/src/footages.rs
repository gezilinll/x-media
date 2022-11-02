use crate::renderer::Renderer;

pub trait FootageSource {
    fn update_texture(&mut self, renderer: &mut Renderer);
}

pub struct AFrameFootage {
    texture: Option<wgpu::Texture>,
    pixels: Vec<u8>,
    width: u32,
    height: u32,
}

impl FootageSource for AFrameFootage {
    fn update_texture(&mut self, renderer: &mut Renderer) {
        let size = wgpu::Extent3d {
            width: self.width,
            height: self.height,
            depth_or_array_layers: 1,
        };
        if (self.texture.is_none()) {
            let tex_des = wgpu::TextureDescriptor {
                label: None,
                size,
                mip_level_count: 1,
                sample_count: 1,
                dimension: wgpu::TextureDimension::D2,
                format: wgpu::TextureFormat::Rgba8UnormSrgb,
                usage: wgpu::TextureUsages::TEXTURE_BINDING | wgpu::TextureUsages::COPY_DST, //上传像素数据一定要有 COPY_DST
            };
        }
    }
}

impl AFrameFootage {
    fn set_pixels(&mut self, pixels: Vec<u8>, width: u32, height: u32) {
        self.pixels = pixels;
        self.width = width;
        self.height = height;
    }
}

pub struct AFootage {
    source: Box<dyn FootageSource>,
}