cmake_minimum_required(VERSION 2.8.12)

project(sysreg C)

add_library(sysreg STATIC sysreg.c sysreg.h)
target_link_libraries(sysreg interrupt)

target_include_directories(sysreg
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS sysreg
        EXPORT sysregTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES sysreg.h DESTINATION include)
