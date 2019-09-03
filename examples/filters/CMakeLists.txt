add_executable(filters filters.cpp)
target_link_libraries(filters GPUImage-X)
set_target_properties(filters PROPERTIES FOLDER "examples")
target_compile_definitions(filters PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")

file(COPY ../runtime/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})