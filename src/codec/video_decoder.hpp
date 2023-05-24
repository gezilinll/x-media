//
// Created by 林炳河 on 2023/5/20.
//

#ifndef X_MEDIA_VIDEO_DECODER_HPP
#define X_MEDIA_VIDEO_DECODER_HPP

#include <string>
#include "ffmpeg_wrapper.hpp"

class VideoDecoder {
public:
    VideoDecoder(std::string filePath);

    ~VideoDecoder();

    std::shared_ptr<AVFrame> receive_frame();

private:
    void open();

    void send_packet();

    void close();

private:
    std::string file_path;
    std::unique_ptr<AVFormatContext, AVFormatContextDeleterForDecoder> format_context;
    int video_stream_index;
    AVStream* video_stream = nullptr;
    std::unique_ptr<AVCodecContext, AVCodecContextDeleter> video_codec_context;
};

#endif  // X_MEDIA_VIDEO_DECODER_HPP
