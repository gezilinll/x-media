#This is free and unencumbered software released into the public domain.

cmake_minimum_required( VERSION 3.0 )
project( GPUImage-X )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory( thirdparty )
include( thirdparty/bgfx.cmake/cmake/util/ConfigureDebugging.cmake )
add_library( GPUImage-X STATIC source/bigg.cpp source/bigg_assets.h source/bigg_imgui.hpp source/bigg_shaders.hpp include/bigg.hpp )
target_link_libraries( GPUImage-X PUBLIC bgfx imgui glfw glm )
target_include_directories( GPUImage-X PUBLIC include )

option( EXAMPLES "Build examples." ON )
if( EXAMPLES )
 add_subdirectory( examples )
endif()

option( BIGG_ASSET_GEN "Generate binary assets." OFF )
if( BIGG_ASSET_GEN )
	add_subdirectory( assets/gen )
endif()