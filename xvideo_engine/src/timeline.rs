use std::sync::{Arc, Mutex};

use crate::tracks::{self, ATrack};

pub struct Timeline {
    tracks : Arc<Mutex<Vec<Arc<ATrack>>>>,
}

impl Timeline {
    pub fn add_track(&mut self, track: Arc<ATrack>) {
        self.tracks.lock().unwrap().push(track.clone());
    }
}

