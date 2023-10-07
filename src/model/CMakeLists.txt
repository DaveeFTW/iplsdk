cmake_minimum_required(VERSION 2.8.12)

project(model C)

add_library(model STATIC model.c model.h)
target_link_libraries(model sysreg syscon)

target_include_directories(model
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS model
        EXPORT modelTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES model.h DESTINATION include)
