cmake_minimum_required(VERSION 3.15)

project(x-media)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src/utils)

aux_source_directory(src/codec DIR_SRCS_CODEC)
include_directories(src/codec)

aux_source_directory(src/player DIR_SRCS_PLAYER)
include_directories(src/player)

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
    ${DIR_SRCS_PLAYER}
)
target_link_libraries(xme libavformat)