cmake_minimum_required(VERSION 3.16)

project(odesa VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(odesa
    src/main.cpp
)

target_include_directories(odesa PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)