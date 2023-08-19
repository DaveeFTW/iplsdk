cmake_minimum_required (VERSION 2.8.12)
project(flash_led C ASM)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -flto -std=gnu99 -Os" )

add_executable(flash_led src/main.c)

target_link_libraries(flash_led uart syscon sysreg gpio kirk spi utils cpu gcc c)

add_custom_command(TARGET flash_led POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary flash_led flash_led.bin
    COMMAND make_ipl flash_led.bin flash_led.ipl reset_block
    # COMMAND make_ipl flash_led.bin reset_blk.ipl reset_block_app_bld
    # COMMAND make_ipl flash_led.bin flash_led.ipl reset_block_app reset_blk.ipl
    # COMMAND make_ipl flash_led.bin flash_led.ipl normal
    # COMMAND make_signed_ipl ${CMAKE_CURRENT_SOURCE_DIR}/bourbon_ipl.bin flash_led.bin flash_led.ipl 0x04000000
)
