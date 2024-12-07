cmake_minimum_required(VERSION 3.1)
project(SnakeGame VERSION 1.0
                  LANGUAGES CXX)

# CXX standard
set(CMAKE_CXX_STANDARD 17)

# third party libraries

# game library
add_library(gamelib STATIC
    lib/game/game.cpp
    lib/game/snake.cpp
)

target_include_directories(gamelib
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# nn library
add_library(nnlib STATIC
    lib/nn/neural_network.cpp
    lib/nn/activation_func.cpp
)

target_include_directories(nnlib
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# GA library
add_library(galib STATIC
    lib/ga/base_ga.cpp
    lib/ga/individual.cpp
)

target_include_directories(galib
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(game src/test_game.cpp)

target_link_libraries(game gamelib)

add_executable(nn src/test_nn.cpp)

target_link_libraries(nn nnlib)

add_executable(ga src/test_ga.cpp)

target_link_libraries(ga galib)