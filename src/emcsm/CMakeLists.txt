cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(emcsm C)

add_library(emcsm STATIC emcsm.c chip.c ecc.c scramble.c state.c emcsm.h)
target_link_libraries(emcsm lcd dmacplus lcdc sysreg cpu)

target_include_directories(emcsm
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET emcsm PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS emcsm
        EXPORT emcsmTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES emcsm.h DESTINATION include)
