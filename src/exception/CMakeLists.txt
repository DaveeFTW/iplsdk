cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(exception ASM)

add_library(exception STATIC exception.c exception.S ebasehandler.S exception.h)
target_link_libraries(exception cpu)

target_include_directories(exception
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET exception PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS exception
        EXPORT exceptionTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES exception.h DESTINATION include)
