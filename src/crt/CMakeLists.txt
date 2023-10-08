cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(crt0 ASM)

add_custom_target(crt0.o ALL
    COMMAND psp-as ${CMAKE_CURRENT_SOURCE_DIR}/crt0.S -o crt0.o
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/crt0.S
)
set_property(TARGET crt0.o PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crt0.o linker.x DESTINATION lib)
