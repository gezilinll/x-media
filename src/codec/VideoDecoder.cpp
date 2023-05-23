//
// Created by 林炳河 on 2023/5/20.
//

#include "VideoDecoder.hpp"

VideoDecoder::VideoDecoder(std::string filePath)
    : mFilePath(filePath), mFormatContext(nullptr), mVideoStream(nullptr) {}

void VideoDecoder::open() {}

void VideoDecoder::close() {}