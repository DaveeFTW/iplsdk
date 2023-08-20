cmake_minimum_required(VERSION 2.8.12)

project(uart C ASM)

add_library(uart STATIC uart.c uart.h)

target_include_directories(uart
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS uart
        EXPORT uartTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES uart.h DESTINATION include)
