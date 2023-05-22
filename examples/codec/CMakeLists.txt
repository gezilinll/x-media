set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../codec MODULE_CODEC.out)
include_directories(../../codec/source)

add_executable(CodecExample main.cpp)
target_link_libraries(CodecExample codec)
