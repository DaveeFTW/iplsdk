cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(spi C)

add_library(spi STATIC spi.c spi.h)

target_include_directories(spi
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET spi PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS spi
        EXPORT spiTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES spi.h DESTINATION include)
