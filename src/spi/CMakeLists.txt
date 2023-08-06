cmake_minimum_required(VERSION 2.8)

project(spi C)

add_library(spi STATIC pl022.c spi.h)

target_include_directories(spi
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS spi
        EXPORT spiTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES spi.h DESTINATION include)
