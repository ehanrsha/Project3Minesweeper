cmake_minimum_required(VERSION 3.28)
project(Project3Minesweeper LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(Project3Minesweeper main.cpp)
target_compile_features(Project3Minesweeper PRIVATE cxx_std_17)
target_link_libraries(Project3Minesweeper PRIVATE sfml-graphics)

