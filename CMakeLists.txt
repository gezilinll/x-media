cmake_minimum_required(VERSION 3.0)

project(GPUImage-X)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 第三方库配置
add_subdirectory(thirdparty)

# 源码配置
aux_source_directory(source DIR_SRCS)
aux_source_directory(source/filter DIR_FILTER)
aux_source_directory(source/inout DIR_INOUT)
aux_source_directory(source/layers DIR_LAYERS)
aux_source_directory(source/particles DIR_PARTICLES)

# 库配置
add_library(GPUImage-X STATIC ${DIR_SRCS} ${DIR_FILTER} ${DIR_INOUT} ${DIR_LAYERS} ${DIR_PARTICLES})
target_link_libraries(GPUImage-X PUBLIC bgfx example-common glm_static )

# 头文件配置
target_include_directories(GPUImage-X PUBLIC include)
target_include_directories(GPUImage-X PUBLIC source)
target_include_directories(GPUImage-X PUBLIC source/filter)
target_include_directories(GPUImage-X PUBLIC source/inout)
target_include_directories(GPUImage-X PUBLIC source/layers)
target_include_directories(GPUImage-X PUBLIC source/particles)
target_include_directories(GPUImage-X PUBLIC thirdparty/bgfx.cmake/bgfx/include)
target_include_directories(GPUImage-X PUBLIC thirdparty/glm)

# Demo 配置
option(EXAMPLES "Build examples." ON)
if (EXAMPLES)
    add_subdirectory(examples)
endif ()
