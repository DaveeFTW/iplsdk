cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(mspro C)

add_library(mspro STATIC mspro.c mspro.h)
target_link_libraries(mspro sysreg)

target_include_directories(mspro
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET mspro PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS mspro
        EXPORT msproTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES mspro.h DESTINATION include)
