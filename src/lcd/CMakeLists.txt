cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(lcd C)

add_library(lcd STATIC lcd.c phatlcd.c hibari.c samantha.c tmdlcd.c streetlcd.c lcd.h)
target_link_libraries(lcd dmacplus pwm interrupt gpio spi sysreg model cpu utils)

target_include_directories(lcd
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET lcd PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS lcd
        EXPORT lcdTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES lcd.h DESTINATION include)
