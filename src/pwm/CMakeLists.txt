cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(pwm C)

add_library(pwm STATIC pwm.c pwm.h)
target_link_libraries(pwm sysreg)

target_include_directories(pwm
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET pwm PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS pwm
        EXPORT pwmTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES pwm.h DESTINATION include)
