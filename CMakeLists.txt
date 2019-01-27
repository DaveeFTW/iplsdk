cmake_minimum_required(VERSION 2.8)

project(iplsdk C ASM)

add_subdirectory(src/crt)
add_subdirectory(src/utils)
add_subdirectory(src/sysreg)
add_subdirectory(src/uart)
add_subdirectory(src/kirk)
