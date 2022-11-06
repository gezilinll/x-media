use rust_fsm::*;

state_machine! {
    pub EngineStatus(IDLE)

    IDLE(Play) => Playing[Success],
    IDLE(Pause) => Paused[Success],
    Paused(Play) => Playing[Success],
    Paused(Stop) => Stopped[Success],
    Playing(Pause) => Paused[Success],
    Playing(Stop) => Stopped[Success],
    Stopped(Play) => Playing[Success],
}
