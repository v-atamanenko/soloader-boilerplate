cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
    else()
        message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
    endif()
endif()

project(SceLibcBridge)
include("${VITASDK}/share/vita.cmake" REQUIRED)

add_custom_target(SceLibcBridge ALL
        COMMAND vita-libs-gen ${CMAKE_CURRENT_SOURCE_DIR}/nids.yml ${CMAKE_CURRENT_BINARY_DIR}/stubs
        COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/stubs
        COMMENT "Building stubs for SceLibcBridge"
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/stubs/
        DESTINATION lib
        FILES_MATCHING PATTERN "*.a"
)
