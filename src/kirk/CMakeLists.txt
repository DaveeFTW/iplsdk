cmake_minimum_required(VERSION 2.8.12)

project(kirk C)

add_library(kirk STATIC kirk.c kirk.h)
target_link_libraries(kirk sysreg cpu utils)

target_include_directories(kirk
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS kirk
        EXPORT kirkTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES kirk.h DESTINATION include)
