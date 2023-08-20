cmake_minimum_required(VERSION 2.8.12)

project(cpu ASM)

add_library(cpu STATIC cache.S cpu.S cpu.h)

target_include_directories(cpu
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS cpu
        EXPORT cpuTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES cpu.h DESTINATION include)
