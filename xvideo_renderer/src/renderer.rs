pub struct Renderer {
    pub device: wgpu::Device,
    pub surface: wgpu::Surface,
    pub queue: wgpu::Queue,
    pub configuration: wgpu::SurfaceConfiguration,
}

impl Renderer {
    pub fn new(
        device: wgpu::Device,
        surface: wgpu::Surface,
        queue: wgpu::Queue,
        configuration: wgpu::SurfaceConfiguration,
    ) -> Self {
        Self {
            device,
            surface,
            queue,
            configuration,
        }
    }
}
