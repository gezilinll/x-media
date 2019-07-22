//
// Created by 林炳河 on 2019/3/4.
//

#ifndef MMLog_h
#define MMLog_h

#include "bgfx/platform.h"

enum {
    LOG_LEVEL_ALL = 0,
    LOG_LEVEL_VERBOSE = 1,
    LOG_LEVEL_DEBUG = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_WARN = 4,
    LOG_LEVEL_ERROR = 5,
    LOG_LEVEL_FATAL = 6,
    LOG_LEVEL_OFF = 7,
};

#define TARGET_LOG_LEVEL (LOG_LEVEL_DEBUG)

#define DEBUG_NATVIE

#ifdef DEBUG_NATVIE

#if BX_PLATFORM_ANDROID

#include <android/log.h>
#define LOG_TAG "XImage"
#define LOGV(...)                                                           \
    do {                                                                    \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_VERBOSE)                       \
            __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__); \
    } while (0)
#define LOGD(...)                                                         \
    do {                                                                  \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_DEBUG)                       \
            __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); \
    } while (0)
#define LOGI(...)                                                        \
    do {                                                                 \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_INFO)                       \
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); \
    } while (0)
#define LOGW(...)                                                        \
    do {                                                                 \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_WARN)                       \
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__); \
    } while (0)
#define LOGE(...)                                                         \
    do {                                                                  \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_ERROR)                       \
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__); \
    } while (0)
#define LOGF(...)                                                         \
    do {                                                                  \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_FATAL)                       \
            __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__); \
    } while (0)

#else

#if BX_PLATFORM_WINDOWS || BX_PLATFORM_OSX \
 || BX_PLATFORM_LINUX || BX_PLATFORM_IOS || BX_PLATFORM_EMSCRIPTEN

#include <stdio.h>

#define LOGV(...)                                       \
    do {                                                \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_VERBOSE) { \
            printf("[XImage] VERBOSE:> ");      \
            printf(__VA_ARGS__);                        \
            printf("\n");                               \
        }                                               \
    } while (0)
#define LOGD(...)                                     \
    do {                                              \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_DEBUG) { \
            printf("<[XImage] DEBUG:> ");     \
            printf(__VA_ARGS__);                      \
            printf("\n");                             \
        }                                             \
    } while (0)
#define LOGI(...)                                    \
    do {                                             \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_INFO) { \
            printf("<[XImage] INFO:> ");     \
            printf(__VA_ARGS__);                     \
            printf("\n");                            \
        }                                            \
    } while (0)
#define LOGW(...)                                    \
    do {                                             \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_WARN) { \
            printf("<[XImage] WARN:> ");     \
            printf(__VA_ARGS__);                     \
            printf("\n");                            \
        }                                            \
    } while (0)
#define LOGE(...)                                     \
    do {                                              \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_ERROR) { \
            printf("<[XImage] ERROR:> ");     \
            printf(__VA_ARGS__);                      \
            printf("\n");                             \
        }                                             \
    } while (0)
#define LOGF(...)                                     \
    do {                                              \
        if (TARGET_LOG_LEVEL <= LOG_LEVEL_FATAL) { \
            printf("<[XImage] FATAL:> ");     \
            printf(__VA_ARGS__);                      \
            printf("\n");                             \
        }                                             \
    } while (0)
#define XTRACE(...) LOGI(__VA_ARGS__)

#endif

#endif

#else

#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#define LOGF(...)

#endif

#endif  // MMLog_h
