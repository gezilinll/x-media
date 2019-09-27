add_executable(layers layers.cpp XEffectUI.cpp XEffectUI.hpp XFilterEffectUI.cpp XFilterEffectUI.hpp XFilterEffectListUI.cpp XFilterEffectListUI.hpp XBlendUI.cpp XBlendUI.hpp)
target_link_libraries(layers GPUImage-X)
set_target_properties(layers PROPERTIES FOLDER "examples")
target_compile_definitions(layers PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")

file(COPY ../runtime/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})