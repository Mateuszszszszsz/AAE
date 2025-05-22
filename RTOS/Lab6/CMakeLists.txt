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
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Lab6 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(include)

# Import FreeRTOS Kernel libraries
include(FreeRTOS_Kernel_import.cmake)

# Add executable. Default name is the project name, version 0.1

add_executable(Lab6 src/Lab6.c src/accelerometer.c src/oled.c src/microphone.c)

pico_set_program_name(Lab6 "Lab6")
pico_set_program_version(Lab6 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Lab6 1)
pico_enable_stdio_usb(Lab6 0)

# Add the standard library to the build
target_link_libraries(Lab6
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(Lab6 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Lab6
        FreeRTOS-Kernel-Heap4
        hardware_i2c
        hardware_dma
        hardware_adc
        )

pico_add_extra_outputs(Lab6)

