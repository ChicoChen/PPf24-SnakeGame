cmake_minimum_required(VERSION 3.1)
project(SnakeGame VERSION 1.0
                  LANGUAGES CXX)

# CXX standard
set(CMAKE_CXX_STANDARD 17)

# Add option to control DEBUG output
option(ENABLE_DEBUG "Enable debug output" ON)
option(GAME_BOARD "Enable game board output" OFF)

# third party libraries

find_package(SDL2 REQUIRED)

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
lib/ga/serial_ga.cpp
lib/ga/individual.cpp
)

target_include_directories(galib
PUBLIC
${PROJECT_SOURCE_DIR}/include
)


# test executables
add_executable(game src/test_game.cpp)

target_link_libraries(game gamelib)

add_executable(nn src/test_nn.cpp)

target_link_libraries(nn nnlib)

add_executable(ga src/test_ga.cpp)

target_link_libraries(ga galib nnlib gamelib)

add_executable(test_snake src/test_game_with_nn.cpp)

target_link_libraries(test_snake gamelib nnlib)

# Conditionally define DEBUG macro based on the option
if(ENABLE_DEBUG)
  target_compile_definitions(gamelib PRIVATE DEBUG)
  target_compile_definitions(nnlib PRIVATE DEBUG)
  target_compile_definitions(galib PRIVATE DEBUG)
  target_compile_definitions(game PRIVATE DEBUG)
  target_compile_definitions(nn PRIVATE DEBUG)
  target_compile_definitions(ga PRIVATE DEBUG)
endif()

if(GAME_BOARD)
  target_compile_definitions(galib PRIVATE SHOWGAMEBOARD)
  target_compile_definitions(game PRIVATE SHOWGAMEBOARD)
  target_compile_definitions(nn PRIVATE SHOWGAMEBOARD)
  target_compile_definitions(ga PRIVATE SHOWGAMEBOARD)
endif()

# Snake Viewer 
add_executable(snake_viewer src/snake_viewer.cpp)
target_include_directories(snake_viewer PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(snake_viewer PRIVATE SDL2::SDL2 gamelib nnlib)
