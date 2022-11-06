use crate::tracks::VideoSource;
use crate::{state::EngineStatus, timeline::Timeline, tracks::ATrack};
use ffmpeg::{format, frame, media, picture};
use ffmpeg::{
    format::Pixel,
    software::scaling::{context::Context, flag::Flags},
    util::frame::video::Video,
};
use rust_fsm::StateMachine;
use std::sync::atomic::AtomicU32;
use std::thread;
use std::{
    sync::{Arc, Mutex},
    thread::JoinHandle,
};
use winit::window::Window;
use xvideo_renderer::footages::AFrameFootage;
use xvideo_renderer::layer::ALayer;
use xvideo_renderer::video_renderer::XVideoRenderer;

pub struct XVideoEngine {
    timeline: Arc<Mutex<Timeline>>,
    state: StateMachine<EngineStatus>,
    renderer: Option<XVideoRenderer>,
    render_thread_handle: Option<JoinHandle<()>>,
    decode_video_thread_handle: Option<JoinHandle<()>>,
    current_clock: Arc<AtomicU32>,
}

impl XVideoEngine {
    pub async fn new(window: &Window) -> Self {
        Self {
            timeline: Arc::new(Mutex::new(Timeline::new())),
            state: StateMachine::new(),
            renderer: Some(XVideoRenderer::new(window).await),
            render_thread_handle: None,
            decode_video_thread_handle: None,
            current_clock: Arc::new(AtomicU32::new(0)),
        }
    }

    pub fn add_track(&mut self, track: Arc<ATrack>) {
        self.timeline.lock().unwrap().add_track(track.clone());
    }

    pub fn play(&mut self) {
        let frames_to_render: Arc<Mutex<Vec<frame::Video>>> = Arc::new(Mutex::new(vec![]));

        {
            let timeline = self.timeline.clone();
            let decoded_frames = frames_to_render.clone();
            self.decode_video_thread_handle = Some(thread::spawn(move || loop {
                let mut ictx = {
                    let tracks_lock = timeline.lock().unwrap().get_tracks();
                    let tracks = tracks_lock.lock().unwrap();
                    let track = tracks[0].clone();
                    let video_source = track
                        .get_source()
                        .as_any()
                        .downcast_ref::<VideoSource>()
                        .unwrap();
                    format::input(&video_source.get_file_path()).unwrap()
                };
                for (ist_index, ist) in ictx.streams().enumerate() {
                    let ist_medium = ist.codec().medium();
                    if ist_medium != media::Type::Audio
                        && ist_medium != media::Type::Video
                        && ist_medium != media::Type::Subtitle
                    {
                        continue;
                    }
                    if ist_medium == media::Type::Video {
                        let mut decoder = ist.codec().decoder().video().unwrap();
                        for (stream, mut packet) in ictx.packets() {
                            if stream.index() == ist_index {
                                packet.rescale_ts(stream.time_base(), decoder.time_base());
                                let mut frame = frame::Video::empty();
                                decoder.decode(&packet, &mut frame).unwrap();
                                let timestamp = frame.timestamp();
                                frame.set_pts(timestamp);
                                frame.set_kind(picture::Type::None);
                                decoded_frames.lock().as_mut().unwrap().push(frame);
                            }
                        }
                        break;
                    }
                }
            }));
        }

        let decoded_frames = frames_to_render.clone();
        let mut renderer = self.renderer.take();
        self.render_thread_handle = Some(thread::spawn(move || loop {
            let renderer = renderer.as_mut().unwrap();
            let frame = { decoded_frames.lock().unwrap().pop() };
            if frame.is_some() {
                let frame = frame.as_ref().unwrap();
                let mut scaler = Context::get(
                    frame.format(),
                    frame.width(),
                    frame.height(),
                    Pixel::RGBA,
                    frame.width(),
                    frame.height(),
                    Flags::BILINEAR,
                )
                .unwrap();

                let mut footage = Box::new(AFrameFootage::new());
                let mut rgb_frame = Video::empty();
                scaler.run(&frame, &mut rgb_frame).unwrap();
                footage.set_pixels(rgb_frame.data(0).to_vec(), frame.width(), frame.height());

                let layer = ALayer::new(0, footage);
                renderer.clear_layers();
                renderer.add_layer(layer);
                renderer.render_frame();
            }
        }));
    }
}
