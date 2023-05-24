//
// Created by 林炳河 on 2023/5/20.
//

#include "video_decoder.hpp"

VideoDecoder::VideoDecoder(std::string file_path)
    : file_path(file_path),
      format_context(nullptr),
      video_stream(nullptr),
      video_stream_index(-1),
      video_codec_context(nullptr) {
    open();
}

VideoDecoder::~VideoDecoder() { close(); }

std::shared_ptr<AVFrame> VideoDecoder::receive_frame() {
    std::shared_ptr<AVFrame> avFrame(av_frame_alloc(), AVFrameDeleter());
    if (!avFrame) {
        printf("video_decoder receiveFrame failed.\n");
        return nullptr;
    }
    int ret;
    while (true) {
        ret = avcodec_receive_frame(video_codec_context.get(), avFrame.get());
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            printf("VideoDecoder avcodec_receive_frame failed. %s\n", av_err2str(ret));
            return nullptr;
        } else if (ret == AVERROR_EOF) {
            return nullptr;
        } else if (ret >= 0) {
            return avFrame;
        }

        send_packet();
    }
}

void VideoDecoder::send_packet() {
    std::shared_ptr<AVPacket> packet(av_packet_alloc(), AVPacketDeleter());
    if (!packet) {
        printf("VideoDecoder av_packet_alloc failed.\n", );
        return;
    }
    do {
        int ret = av_read_frame(format_context.get(), packet.get());
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                packet->data = nullptr;
                packet->size = 0;
            } else {
                printf("VideoDecoder av_read_frame failed. %s\n", av_err2str(ret));
                return;
            }
            break;
        }
        if (packet->stream_index == video_stream_index) {
            av_packet_rescale_ts(packet.get(), video_stream->time_base, {1, 1000});
            break;
        }
    } while (true);
    if (packet) {
        // when avcodec_send_packet return EAGAIN, we have to send packet after
        // avcodec_receive_frame https://ffmpeg.org/doxygen/4.1/group__lavc__encdec.html
        int ret = avcodec_send_packet(video_codec_context.get(), packet.get());
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            printf("VideoDecoder avcodec_send_packet failed. %s\n", av_err2str(ret));
        }
    }
}

void VideoDecoder::open() {
    AVFormatContext* formatContext = nullptr;
    int ret = avformat_open_input(&formatContext, file_path.data(), nullptr, nullptr);
    if (ret < 0) {
        printf("video_decoder avformat_open_input failed.\n");
        return;
    }
    auto ptrFormatContext
        = std::unique_ptr<AVFormatContext, AVFormatContextDeleterForDecoder>(formatContext);
    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        printf("video_decoder avformat_find_stream_info failed.\n");
        return;
    }
    if (formatContext->nb_streams <= 0) {
        printf("video_decoder nb_streams <= 0 \n");
        return;
    }
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        formatContext->streams[i]->discard = AVDISCARD_ALL;
    }

    int videoStreamIndex
        = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (videoStreamIndex >= 0) {
        format_context = std::move(ptrFormatContext);
        video_stream_index = videoStreamIndex;
        video_stream = formatContext->streams[videoStreamIndex];
        video_stream->discard = AVDISCARD_DEFAULT;

        format_context->streams[video_stream_index]->discard = AVDISCARD_DEFAULT;
        AVCodec* videoCodec
            = avcodec_find_decoder(format_context->streams[video_stream_index]->codecpar->codec_id);
        if (!videoCodec) {
            printf("VideoDecoder avcodec_find_decoder failed.\n");
            return;
        }

        auto videoCodecContext = std::unique_ptr<AVCodecContext, AVCodecContextDeleter>(
            avcodec_alloc_context3(videoCodec));
        if (!videoCodecContext) {
            printf("VideoDecoder avcodec_alloc_context3 failed.\n");
            return;
        }
        ret = avcodec_parameters_to_context(videoCodecContext.get(),
                                            format_context->streams[video_stream_index]->codecpar);
        if (ret < 0) {
            printf("VideoDecoder avcodec_parameters_to_context failed.\n");
            return;
        }
        AVDictionary* opts = nullptr;
        av_dict_set(&opts, "threads", "auto", 0);
        ret = avcodec_open2(videoCodecContext.get(), videoCodec, &opts);
        if (opts) {
            av_dict_free(&opts);
        }
        if (ret < 0) {
            printf("VideoDecoder avcodec_open2 failed. %s\n", av_err2str(ret));
            return;
        }
        video_codec_context = std::move(videoCodecContext);
    } else {
        printf("video_decoder can not find video stream.\n");
        return;
    }
}

void VideoDecoder::close() {
    avcodec_flush_buffers(video_codec_context.get());
    video_codec_context = nullptr;
    video_stream_index = -1;
    video_stream = nullptr;
    format_context = nullptr;
}