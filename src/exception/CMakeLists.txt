cmake_minimum_required(VERSION 2.8)

project(exception ASM)

add_library(exception STATIC exception.c exception.S exception.h)

target_include_directories(exception
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS exception
        EXPORT exceptionTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES exception.h DESTINATION include)
