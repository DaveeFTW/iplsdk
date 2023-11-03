cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(led C)

add_library(led STATIC led.c led.h)
target_link_libraries(led interrupt model syscon gpio sysreg cpu)

target_include_directories(led
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET led PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS led
        EXPORT ledTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES led.h DESTINATION include)
