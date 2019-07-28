add_executable(particles particles.cpp)
target_link_libraries(particles GPUImage-X example-common)
set_target_properties(particles PROPERTIES FOLDER "examples")
target_include_directories(particles PUBLIC ../../thirdparty/bgfx.cmake/bgfx/examples/common)
target_compile_definitions(particles PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")

configure_debugging(particles WORKING_DIR ../runtime)
