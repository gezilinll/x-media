add_executable( filters filters.cpp )
target_link_libraries( filters GPUImage-X )
set_target_properties( filters PROPERTIES FOLDER "examples" )

configure_debugging( filters WORKING_DIR ../runtime )
