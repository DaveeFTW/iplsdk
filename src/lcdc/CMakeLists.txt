cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(lcdc C)

add_library(lcdc STATIC lcdc.c lcdc.h)
target_link_libraries(lcdc interrupt model sysreg)

target_include_directories(lcdc
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET lcdc PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS lcdc
        EXPORT lcdcTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES lcdc.h DESTINATION include)
