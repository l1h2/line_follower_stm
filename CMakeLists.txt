cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME line_follower)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# List module directory names under Core/
set(USER_MODULES
    state_machine
    hal
    logger
    timer
    led
    sensors
    motors
    pid
    serial
    turbine
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

foreach(mod ${USER_MODULES})
    set(MOD_SRC_DIR "${CMAKE_SOURCE_DIR}/Core/${mod}/src")
    set(MOD_INC_DIR "${CMAKE_SOURCE_DIR}/Core/${mod}/include")

    if(EXISTS "${MOD_SRC_DIR}")
        file(GLOB_RECURSE MOD_SRCS CONFIGURE_DEPENDS "${MOD_SRC_DIR}/*.c")
        if(MOD_SRCS)
            target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MOD_SRCS})
        endif()
    endif()

    if(EXISTS "${MOD_INC_DIR}")
        target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${MOD_INC_DIR}")
    endif()
endforeach()

# (Optional) Global compile options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
