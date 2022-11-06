use crate::tracks::ATrack;
use std::sync::{Arc, Mutex};

pub struct Timeline {
    tracks: Arc<Mutex<Vec<Arc<ATrack>>>>,
}
unsafe impl Send for Timeline {}

impl Timeline {
    pub fn new() -> Self {
        Self {
            tracks: Arc::new(Mutex::new(vec![])),
        }
    }

    pub fn add_track(&mut self, track: Arc<ATrack>) {
        self.tracks.lock().unwrap().push(track.clone());
    }

    pub fn get_tracks(&self) -> Arc<Mutex<Vec<Arc<ATrack>>>> {
        return self.tracks.clone();
    }
}
