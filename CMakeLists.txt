cmake_minimum_required(VERSION 3.16)

project(odessa VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(odessa_example
    examples/basic_world.cpp
    src/utils/uuid.cpp
    src/core/entity/entity.cpp
    src/core/world/world.cpp
    src/core/runtime/runtime.cpp
    src/transport/mqtt_client.cpp
    src/core/coordinate/transform.cpp
    src/core/serialization/entity_json.cpp
    src/core/command/command.cpp
)

target_include_directories(odessa_example PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(odessa_example PRIVATE Boost::boost)
target_link_libraries(odessa_example PRIVATE nlohmann_json::nlohmann_json
)
