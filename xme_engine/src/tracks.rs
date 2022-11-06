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
    pub start_time_in_timeline: i32,
    pub duration_in_timeline: i32,
    pub source: Box<dyn TrackSource>,
}
unsafe impl Send for ATrack {}

impl ATrack {
    pub fn new(id: i64, source: Box<dyn TrackSource>) -> Self {
        Self {
            id,
            start_time_in_timeline: 0,
            duration_in_timeline: 0,
            source,
        }
    }

    pub fn is_in_time_range(&self, clock: u32) -> bool {
        return clock >= self.start_time_in_timeline.try_into().unwrap()
            && clock <= self.get_end_time_in_timeline().try_into().unwrap();
    }

    pub fn get_end_time_in_timeline(&self) -> i32 {
        return self.start_time_in_timeline + self.duration_in_timeline;
    }

    pub fn get_source(&self) -> &Box<dyn TrackSource> {
        return &self.source;
    }
}

pub struct AudioSource {
    file_path: String,
    self_start_time: i32,
    self_duration: i32,
    enable_loop: bool,
}

pub struct VideoSource {
    file_path: String,
    self_start_time: i32,
    self_duration: i32,
    enable_loop: bool,
}

pub struct ImageSource {
    file_path: String,
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

impl VideoSource {
    pub fn new(file_path: String) -> Self {
        Self {
            file_path,
            self_start_time: 0,
            self_duration: 0,
            enable_loop: false,
        }
    }

    pub fn get_file_path(&self) -> String {
        return self.file_path.clone();
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
