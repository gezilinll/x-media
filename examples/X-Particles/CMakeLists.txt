add_executable(X-Particles XParticlesExample.cpp)
target_link_libraries(X-Particles GPUImage-X)
set_target_properties(X-Particles PROPERTIES FOLDER "examples")
target_compile_definitions(X-Particles PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")

file(COPY ../runtime/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})