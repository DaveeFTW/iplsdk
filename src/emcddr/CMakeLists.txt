cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(emcddr C)

add_library(emcddr STATIC emcddr.c emcddr.h)
target_link_libraries(emcddr sysreg cpu)

target_include_directories(emcddr
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET emcddr PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS emcddr
        EXPORT emcddrTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES emcddr.h DESTINATION include)
