//
// Created by 林炳河 on 2023/5/20.
//

#include "VideoDecoder.hpp"
extern "C" {
#include <libavformat/avformat.h>
}

VideoDecoder::VideoDecoder(std::string filePath): mFilePath(filePath) {
    AVFormatContext* formatContext = nullptr;
    int ret = avformat_open_input(&formatContext, mFilePath.data(), nullptr, nullptr);
}