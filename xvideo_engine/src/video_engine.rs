use std::sync::{Mutex, Arc};

use crate::{timeline::{self, Timeline}, tracks::ATrack};

pub struct XVideoEngine {
    timeline : Arc<Mutex<Timeline>>,
}

impl XVideoEngine {
    pub fn add_track(&mut self, track: Arc<ATrack>) {
        self.timeline.lock().unwrap().add_track(track.clone());
    }

    pub fn refresh(&mut self) {

    }

    pub fn play(&mut self) {
        
    }
}