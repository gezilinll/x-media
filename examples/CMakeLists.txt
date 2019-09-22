add_subdirectory( filters )
add_subdirectory( particles )
add_subdirectory( layers )
target_compile_definitions(GPUImage-X PRIVATE -DENTRY_CONFIG_WORKING_DIRECTORY="${PROJECT_SOURCE_DIR}/examples/runtime/")