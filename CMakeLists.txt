cmake_minimum_required(VERSION 3.1)
project(SnakeGame VERSION 1.0
                  LANGUAGES CXX)

# CXX standard
set(CMAKE_CXX_STANDARD 17)

# locates the OpenMP package
find_package(OpenMP REQUIRED)

if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Add option to control DEBUG output
option(ENABLE_DEBUG "Enable debug output" ON)
option(ENABLE_GAME_BOARD "Enable game board output" OFF)

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
lib/ga/individual.cpp
lib/ga/base_ga.cpp
lib/ga/serial_ga.cpp
lib/ga/openmp_ga.cpp
)

target_include_directories(galib
PUBLIC
${PROJECT_SOURCE_DIR}/include
)

add_executable(game src/test_game.cpp)

target_link_libraries(game gamelib)

add_executable(nn src/test_nn.cpp)

target_link_libraries(nn nnlib)

add_executable(test_serial src/test_serial.cpp)

target_link_libraries(test_serial galib nnlib gamelib)

add_executable(test_openmp src/test_openmp.cpp)


add_executable(test_snake src/test_game_with_nn.cpp)

target_link_libraries(test_snake gamelib nnlib)

# Conditionally define DEBUG macro based on the option
if(ENABLE_DEBUG)
  add_compile_definitions(DEBUG)
endif()

if(ENABLE_GAME_BOARD)
  add_compile_definitions(GAME_BOARD)
endif()

if(OpenMP_CXX_FOUND)
  target_link_libraries(galib PRIVATE OpenMP::OpenMP_CXX)
  target_link_libraries(test_openmp PRIVATE OpenMP::OpenMP_CXX galib nnlib gamelib)
endif()