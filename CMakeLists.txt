# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(roomMonitor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(roomMonitor src/roomMonitor.c lib/tm1637/tm1637.c lib/scd41/scd4x_i2c.c lib/scd41/sensirion_common.c lib/scd41/sensirion_i2c.c lib/scd41/sensirion_i2c_hal.c)

pico_set_program_name(roomMonitor "roomMonitor")
pico_set_program_version(roomMonitor "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(roomMonitor 0)
pico_enable_stdio_usb(roomMonitor 1)

# Add the standard library to the build
target_link_libraries(
        roomMonitor
        pico_stdlib
        hardware_gpio
        hardware_i2c
        hardware_timer
        pico_multicore
)

# Add the standard include files to the build
target_include_directories(roomMonitor PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/lib/scd41

)

pico_add_extra_outputs(roomMonitor)

