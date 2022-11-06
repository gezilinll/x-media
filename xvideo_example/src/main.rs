use std::sync::Arc;

use winit::{event_loop::EventLoop, window::WindowBuilder};
use xvideo_engine::{video_engine::XVideoEngine, tracks::{ATrack, VideoSource}};

fn main() {
    let event_loop = EventLoop::new();
    let window = WindowBuilder::new().build(&event_loop).unwrap();
    let mut engine = pollster::block_on(XVideoEngine::new(&window));
    let file_source = VideoSource::new("/Users/linbinghe/Desktop/1.mp4".into());
    let track = ATrack::new(0, Box::new(file_source));
    engine.add_track(Arc::new(track));
    engine.play();

    event_loop.run(move |event, _, _| {
        match event {
            _ => {}
        }
    });
}
