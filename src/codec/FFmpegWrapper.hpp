//
// Created by 林炳河 on 2023/5/23.
//

#ifndef X_MEDIA_FFMPEGWRAPPER_HPP
#define X_MEDIA_FFMPEGWRAPPER_HPP

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavformat/avformat.h>
#include <libavutil/display.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
#include <libavutil/mathematics.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}
#include <functional>
#include <memory>

struct AVFormatContextDeleterForDecoder {
    void operator()(AVFormatContext* context) {
        avformat_close_input(&context);
        context = nullptr;
    }
};

struct AVFormatContextDeleterForEncoder {
    void operator()(AVFormatContext* context) {
        avformat_free_context(context);
        context = nullptr;
    }
};

struct AVCodecContextDeleter {
    void operator()(AVCodecContext* context) {
        avcodec_free_context(&context);
        context = nullptr;
    }
};

struct SwsContextDeleter {
    void operator()(SwsContext* sws) {
        sws_freeContext(sws);
        sws = nullptr;
    }
};

struct SwrContextDeleter {
    void operator()(SwrContext* swr) {
        swr_free(&swr);
        swr = nullptr;
    }
};

struct AVPacketDeleter {
    void operator()(AVPacket* packet) {
        av_packet_free(&packet);
        packet = nullptr;
    }
};

struct AVFrameDeleter {
    void operator()(AVFrame* frame) {
        av_frame_free(&frame);
        frame = nullptr;
    }
};

#endif  // X_MEDIA_FFMPEGWRAPPER_HPP
