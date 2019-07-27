add_executable(filters filters.cpp)
target_link_libraries(filters GPUImage-X example-common)
set_target_properties(filters PROPERTIES FOLDER "examples")
target_include_directories(filters PUBLIC ../../thirdparty/bgfx.cmake/bgfx/examples/common)
target_compile_definitions(filters PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")
target_compile_definitions(GPUImage-X PRIVATE -DENTRY_CONFIG_WORKING_DIRECTORY="${PROJECT_SOURCE_DIR}/examples/runtime/")

configure_debugging(filters WORKING_DIR ../runtime)
