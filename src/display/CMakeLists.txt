cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(display C)

add_library(display STATIC display.c display.h)
target_link_libraries(display lcd dmacplus lcdc interrupt sysreg cpu)

target_include_directories(display
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET display PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS display
        EXPORT displayTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES display.h DESTINATION include)
