cmake_minimum_required(VERSION 2.8)

project(interrupt ASM)

add_library(interrupt STATIC interrupt.c interrupt.S interrupt.h)

target_include_directories(interrupt
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS interrupt
        EXPORT interruptTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES interrupt.h DESTINATION include)
