//
// Created by 林炳河 on 2023/5/20.
//

#ifndef X_MEDIA_VIDEODECODER_HPP
#define X_MEDIA_VIDEODECODER_HPP

#include <string>
#include "FFmpegWrapper.hpp"

class VideoDecoder {
public:
    VideoDecoder(std::string filePath);

    void open();

    void close();

private:
    std::string mFilePath;
    std::unique_ptr<AVFormatContext, AVFormatContextDeleterForDecoder> mFormatContext;
    AVStream* mVideoStream = nullptr;
};

#endif  // X_MEDIA_VIDEODECODER_HPP
