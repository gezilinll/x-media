cmake_minimum_required(VERSION 3.15)

project(x-media)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(src/codec DIR_SRCS_CODEC)
include_directories(src/codec)

include_directories(3rdparty/ffmpeg/include)
add_library(libavformat SHARED IMPORTED)
set_target_properties(
    libavformat PROPERTIES
    IMPORTED_LOCATION_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ffmpeg/lib/libavformat.dylib
    IMPORTED_LOCATION
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ffmpeg/lib/libavformat.dylib
)

add_library(
    xme
    SHARED
    ${DIR_SRCS_CODEC}
)
target_link_libraries(xme libavformat)