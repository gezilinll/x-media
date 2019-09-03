add_executable(particles particles.cpp)
target_link_libraries(particles GPUImage-X)
set_target_properties(particles PROPERTIES FOLDER "examples")
target_compile_definitions(particles PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")

file(COPY ../runtime/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})