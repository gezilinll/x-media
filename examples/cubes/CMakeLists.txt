add_executable( cubes cubes.cpp fs_cubes.sc vs_cubes.sc )
target_link_libraries( cubes GPUImage-X )
set_target_properties( cubes PROPERTIES FOLDER "examples" )

add_shader( vs_cubes.sc VERTEX   OUTPUT ../runtime/shaders )
add_shader( fs_cubes.sc FRAGMENT OUTPUT ../runtime/shaders )

configure_debugging( cubes WORKING_DIR ../runtime )
