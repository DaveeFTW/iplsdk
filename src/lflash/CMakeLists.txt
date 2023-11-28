cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(lflash C)

add_library(lflash STATIC lflash.c lflash.h)
target_link_libraries(lflash emcsm cpu)

target_include_directories(lflash
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET lflash PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS lflash
        EXPORT lflashTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES lflash.h DESTINATION include)
