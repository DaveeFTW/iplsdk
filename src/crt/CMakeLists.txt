cmake_minimum_required(VERSION 2.8.12)

project(crt0 ASM)

add_custom_target(crt0.o ALL
    COMMAND psp-as ${CMAKE_CURRENT_SOURCE_DIR}/crt0.S -o crt0.o
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/crt0.S
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crt0.o linker.x DESTINATION lib)
