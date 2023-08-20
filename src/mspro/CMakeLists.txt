cmake_minimum_required(VERSION 2.8.12)

project(mspro C)

add_library(mspro STATIC mspro.c mspro.h)
target_link_libraries(mspro sysreg)

target_include_directories(mspro
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS mspro
        EXPORT msproTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES mspro.h DESTINATION include)
