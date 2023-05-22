//
// Created by 林炳河 on 2023/5/20.
//

#include <stdio.h>
#include <memory>
#include "VideoDecoder.hpp"


int main(int argc, char* argv[]) {
    auto decoder = std::make_shared<VideoDecoder>("/Users/linbinghe/Desktop/photo-1593085512500-5d55148d6f0d.jpeg");
    printf("hhhh\n");
}