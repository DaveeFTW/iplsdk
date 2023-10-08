cmake_minimum_required(VERSION 3.9.4)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

project(model C)

add_library(model STATIC model.c model.h)
target_link_libraries(model sysreg syscon)

target_include_directories(model
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_property(TARGET model PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS model
        EXPORT modelTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES model.h DESTINATION include)
