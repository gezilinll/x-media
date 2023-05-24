#include "player.hpp"

Player::Player(std::string filePath) { decoder = std::make_unique<VideoDecoder>(filePath); }

void Player::start() {

}