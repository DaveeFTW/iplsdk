cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(gpio C)

add_library(gpio STATIC gpio.c gpio.h)
target_link_libraries(gpio sysreg cpu)

target_include_directories(gpio
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET gpio PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS gpio
        EXPORT gpioTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES gpio.h DESTINATION include)
