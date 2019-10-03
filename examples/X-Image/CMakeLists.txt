add_executable(X-Image XImageExample.cpp XEffectUI.cpp XEffectUI.hpp XFilterEffectUI.cpp XFilterEffectUI.hpp XFilterEffectListUI.cpp XFilterEffectListUI.hpp XMixtureUI.cpp XMixtureUI.hpp)
target_link_libraries(X-Image GPUImage-X)
set_target_properties(X-Image PROPERTIES FOLDER "examples")
target_compile_definitions(X-Image PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1")

file(COPY ../runtime/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})