add_executable(filters filters.cpp)
target_link_libraries(filters GPUImage-X example-common)
set_target_properties(filters PROPERTIES FOLDER "examples")
target_include_directories(filters PUBLIC ../../thirdparty/bgfx.cmake/bgfx/examples/common)
target_compile_definitions(filters PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")

configure_debugging(filters WORKING_DIR ../runtime)
