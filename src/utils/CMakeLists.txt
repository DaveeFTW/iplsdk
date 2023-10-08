cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(utils C ASM)

add_library(utils STATIC utils.c delay.S utils.h)

target_include_directories(utils
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET utils PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS utils
        EXPORT utilsTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES utils.h DESTINATION include)
