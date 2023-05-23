//
// Created by 林炳河 on 2023/5/20.
//

#include "VideoDecoder.hpp"
extern "C"
{
#include <libavformat/avformat.h>
}

VideoDecoder::VideoDecoder(std::string filePath) : mFilePath(filePath)
{
    printf("VideoDecoder constructor\n");
    AVFormatContext *formatContext = nullptr;
    int ret = avformat_open_input(&formatContext, mFilePath.data(), nullptr, nullptr);
    printf("VideoDecoder ret:%d\n", ret);
    printf("VideoDecoder duration:%lld\n", (long long)formatContext->duration);
}