cmake_minimum_required(VERSION 2.8.12)

project(utils C ASM)

add_library(utils STATIC utils.c utils.h)

target_include_directories(utils
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS utils
        EXPORT utilsTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES utils.h DESTINATION include)
