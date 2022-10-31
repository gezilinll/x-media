use std::any::Any;

pub enum TrackType {
     Audio,
     Video,
     Image,
}

pub trait TrackSource {
    fn get_type(&self) -> TrackType;

    fn as_any(&self) -> &dyn Any;
}

pub struct ATrack {
    pub id: i64,
    pub startTimeInTimeline : i32,
    pub durationInTimeline : i32,
    pub source : Box<dyn TrackSource>,
}

pub struct AudioSource {
    file_path : String,
    self_start_time: i32,
    self_duration: i32,
    enable_loop: bool,
}

pub struct VideoSource {
    file_path : String,
    self_start_time: i32,
    self_duration: i32,
    enable_loop: bool,
}

pub struct ImageSource {
    file_path : String,
}

impl TrackSource for AudioSource {
    fn get_type(&self) -> TrackType {
        return TrackType::Audio;
    }

    fn as_any(&self) -> &dyn Any {
        self
    }
}

impl TrackSource for VideoSource {
    fn get_type(&self) -> TrackType {
        return TrackType::Video;
    }

    fn as_any(&self) -> &dyn Any {
        self
    }
}

impl TrackSource for ImageSource {
    fn get_type(&self) -> TrackType {
        return TrackType::Image;
    }

    fn as_any(&self) -> &dyn Any {
        self
    }
}