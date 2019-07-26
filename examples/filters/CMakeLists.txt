add_executable( filters filters.cpp )
target_link_libraries( filters GPUImage-X example-common )
set_target_properties( filters PROPERTIES FOLDER "examples" )
target_include_directories(filters PUBLIC ../../thirdparty/bgfx.cmake/bgfx/examples/common )

configure_debugging( filters WORKING_DIR ../runtime )
