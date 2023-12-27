cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(sysreg C)

add_library(sysreg STATIC sysreg.c sysreg.h)
target_link_libraries(sysreg cpu)

target_include_directories(sysreg
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET sysreg PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS sysreg
        EXPORT sysregTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES sysreg.h DESTINATION include)
