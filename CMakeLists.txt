cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
enable_language(CXX)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


set(glibc_old "/home/iscemy/Desktop/glibc-2.22")
set(kernel_path "/home/iscemy/Desktop/samsung_t110_kernel/android_kernel_samsung_goyawifi-master")

#SET(CMAKE_C_FLAGS   "-I ${glibc_old}/include -mfloat-abi=softfp -std=c11 -L ${glibc_old}/lib -march=armv7 -I /mnt/d/tolinodev/kernel/kernel_imx/include/ -I /mnt/d/tolinodev/kernel/kernel_imx/ -pthread")
                    
SET(CMAKE_EXE_LINKER_FLAGS  "--static ")
# SET(CMAKE_C_FLAGS "-Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-label -I ${glibc_old}/include -I /mnt/d/tolinodev/kernel/kernel_imx/include/ -I ${kernel_path}/include -I ${kernel_path} \
# -std=c99 -I ${kernel_path}/arch/arm/include/ -I ${kernel_path}/include/asm-generic")



add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})


# set the project name
project(t2s)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/debug) 
add_subdirectory(hagl-hal)
add_subdirectory(hagl)
add_subdirectory(remote-access)
add_subdirectory(app)


