# Assuming your main source file is helloworld.c
add_library(helloworld_module helloworld.c)
target_include_directories(helloworld_module PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include)

# For kernel driver, you may need custom build commands
add_custom_target(build_helloworld 
    COMMAND make -C /lib/modules/${KERNEL_VERSION}/build 
    M=${CMAKE_CURRENT_SOURCE_DIR} modules)

    