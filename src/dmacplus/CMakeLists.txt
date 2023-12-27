cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(dmacplus C)

add_library(dmacplus STATIC dmacplus.c dmacplus.h)
target_link_libraries(dmacplus cpu sysreg syscon)

target_include_directories(dmacplus
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET dmacplus PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS dmacplus
        EXPORT dmacplusTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES dmacplus.h DESTINATION include)
