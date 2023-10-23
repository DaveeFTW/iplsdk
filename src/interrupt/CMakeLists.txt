cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(interrupt ASM)

add_library(interrupt STATIC interrupt.c interrupt.S interrupt.h)
target_link_libraries(interrupt exception cpu)

target_include_directories(interrupt
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET interrupt PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS interrupt
        EXPORT interruptTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES interrupt.h DESTINATION include)
