add_subdirectory( X-Image )
add_subdirectory( X-Particles )
target_compile_definitions(GPUImage-X PRIVATE -DENTRY_CONFIG_WORKING_DIRECTORY="${PROJECT_SOURCE_DIR}/examples/runtime/")