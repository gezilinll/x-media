//
// Created by 林炳河 on 2023/5/23.
//

#ifndef X_MEDIA_PLAYER_HPP
#define X_MEDIA_PLAYER_HPP

#include <memory>
#include "VideoDecoder.hpp"

class Player {
public:
    Player(std::string filePath);

    void start();
private:
    std::unique_ptr<VideoDecoder> mDecoder;
};

#endif  // X_MEDIA_PLAYER_HPP
