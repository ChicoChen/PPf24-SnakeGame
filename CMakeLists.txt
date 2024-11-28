cmake_minimum_required(VERSION 3.1)
project(SnakeGame VERSION 1.0
                  LANGUAGES CXX)

add_library(gamelib STATIC
    lib/game/game.cpp
    lib/game/snake.cpp
)

target_include_directories(gamelib
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(game src/main.cpp)

target_link_libraries(game gamelib)
