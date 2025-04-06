cmake_minimum_required(VERSION 3.10)

project(haikalxx VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

include_directories(inc)
# include_directories(extern/saha/src)
add_executable(${PROJECT_NAME}
    src/main.cpp
    # src/fileops.c
    # src/Component.c
)
# add_subdirectory(extern) # in case of modules...

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
endif()

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}/resource)
