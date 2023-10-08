cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(uart C ASM)

add_library(uart STATIC uart.c uart.h)
target_link_libraries(uart sysreg)

target_include_directories(uart
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET uart PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS uart
        EXPORT uartTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES uart.h DESTINATION include)
