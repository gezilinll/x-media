//
// Created by 林炳河 on 2023/5/20.
//

#ifndef X_MEDIA_VIDEODECODER_HPP
#define X_MEDIA_VIDEODECODER_HPP

#include <string>

class VideoDecoder {
public:
    VideoDecoder(std::string filePath);

private:
    std::string mFilePath;
};

extern "C" long My_Test(char *a) {
    return (long)(new VideoDecoder(std::string(a)));
}

#endif //X_MEDIA_VIDEODECODER_HPP
