add_executable( imgui_demo imgui_demo.cpp )
target_link_libraries( imgui_demo GPUImage-X )
set_target_properties( imgui_demo PROPERTIES FOLDER "examples" )
