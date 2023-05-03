cmake_minimum_required(VERSION 3.20.0)

#Setting the C compiler to GCC
set(CMAKE_C_COMPILER "gcc")
#using C/C++ version 17
set(CMAKE_CXX_STANDARD 17)
#project name, explicitly compiling only C code
project(IOT C)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#using provided IOT library
include_directories(${PROJECT_SOURCE_DIR}/drivers)
find_library(IOT_LIB libIoTLibrary "${PROJECT_SOURCE_DIR}/drivers")
#debugging
message(${IOT_LIB})

#using avr library
include_directories(${PROJECT_SOURCE_DIR}/avr)
include_directories(${PROJECT_SOURCE_DIR}/util)
find_library(AVR_LIB libc "${PROJECT_SOURCE_DIR}/avr")
#debugging
message(${AVR_LIB})

#defining all of our source code as a variable
file(GLOB CUSTOM_SOURCE_CODE
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )
include_directories(${PROJECT_SOURCE_DIR}/include)


#final executable from all source files
add_executable(${PROJECT_NAME} ${CUSTOM_SOURCE_CODE})
#global libraries
target_include_directories(${PROJECT_NAME} PUBLIC ./)
# linking iot drivers to executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${IOT_LIB})
# linking avr drivers to executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${AVR_LIB})