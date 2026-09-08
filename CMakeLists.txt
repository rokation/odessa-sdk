cmake_minimum_required(VERSION 3.16)

project(odessa VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)

add_executable(odessa
    src/main.cpp
    src/utils/uuid.cpp
    src/core/entity/entity.cpp
)


target_include_directories(odessa PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(odessa PRIVATE Boost::boost)