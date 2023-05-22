cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(source DIR_SRCS)
include_directories(source)

include_directories(../3rdparty/ffmpeg/include)
add_library(libavformat SHARED IMPORTED)
set_target_properties(
        libavformat PROPERTIES
        IMPORTED_LOCATION_DEBUG
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/ffmpeg/lib/libavformat.dylib
        IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/ffmpeg/lib/libavformat.dylib
)

add_library(
        codec
        SHARED
        ${DIR_SRCS}
)
target_link_libraries(codec libavformat)