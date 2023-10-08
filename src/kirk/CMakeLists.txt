cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(kirk C)

add_library(kirk STATIC kirk.c kirk.h)
target_link_libraries(kirk sysreg cpu utils)

target_include_directories(kirk
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET kirk PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS kirk
        EXPORT kirkTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES kirk.h DESTINATION include)
