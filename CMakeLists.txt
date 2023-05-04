cmake_minimum_required(VERSION 3.26)
project(asymmetry LANGUAGES C)

# SDL
find_package(SDL2 REQUIRED)
include_directories(asymmetry ${SDL2_INCLUDE_DIRS})

file(GLOB src "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(asymmetry ${src})
target_link_libraries(asymmetry ${SDL2_LIBRARIES})
