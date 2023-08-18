cmake_minimum_required(VERSION 2.8)

project(syscon C)

add_library(syscon STATIC comms.c handshake.c syscon.c syscon.h)
target_link_libraries(syscon gpio spi kirk sysreg)

target_include_directories(syscon
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS syscon
        EXPORT sysconTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES syscon.h DESTINATION include)
