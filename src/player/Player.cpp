#include "Player.hpp"

Player::Player(std::string filePath) { mDecoder = std::make_unique<VideoDecoder>(filePath); }

void Player::start() {

}