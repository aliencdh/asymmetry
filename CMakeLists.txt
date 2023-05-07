cmake_minimum_required(VERSION 3.26)
project(asymmetry LANGUAGES CXX)

# SDL
find_package(SDL2 REQUIRED)
include_directories(asymmetry ${SDL2_INCLUDE_DIRS})

file(GLOB src "${CMAKE_SOURCE_DIR}/src/*.cpp")
set(CMAKE_BUILD_TYPE DEBUG)

add_executable(asymmetry ${src})
target_link_libraries(asymmetry ${SDL2_LIBRARIES})

set_target_properties(asymmetry
    PROPERTIES CMAKE_CXX_STANDARD 20)
