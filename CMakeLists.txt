cmake_minimum_required(VERSION 3.1)
project(SnakeGame VERSION 1.0
                  LANGUAGES CXX)

# CXX standard
set(CMAKE_CXX_STANDARD 17)

# locates the OpenMP package
find_package(OpenMP REQUIRED)

# Add option to control DEBUG output
option(ENABLE_DEBUG "Enable debug output" ON)
option(ENABLE_GAME_BOARD "Enable game board output" OFF)

# third party libraries
if(OpenMP_CXX_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.2
)
FetchContent_MakeAvailable(SDL2)

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
lib/ga/logger.cpp
lib/ga/individual.cpp
lib/ga/base_ga.cpp
lib/ga/serial_ga.cpp
lib/ga/openmp_ga.cpp
)

target_include_directories(galib
PUBLIC
${PROJECT_SOURCE_DIR}/include
)


# test executables
add_executable(tui_game src/test_game.cpp)
target_link_libraries(tui_game gamelib)

add_executable(nn src/test_nn.cpp)
target_link_libraries(nn nnlib)

add_executable(test_serial src/test_serial.cpp)
target_link_libraries(test_serial galib nnlib gamelib)

add_executable(test_openmp src/test_openmp.cpp)
if(OpenMP_CXX_FOUND)
  target_link_libraries(galib PRIVATE OpenMP::OpenMP_CXX)
  target_link_libraries(test_openmp PRIVATE OpenMP::OpenMP_CXX galib nnlib gamelib)
endif()

add_executable(snake_tui src/test_game_with_nn.cpp)
target_link_libraries(snake_tui gamelib nnlib)

# Conditionally define DEBUG macro based on the option
if(ENABLE_DEBUG)
  add_compile_definitions(DEBUG)
endif()

if(ENABLE_GAME_BOARD)
  add_compile_definitions(GAME_BOARD)
endif()

# Snake Viewer
# include_directories(${SDL2_INCLUDE_DIR})
add_executable(snake_gui src/snake_viewer.cpp)
# target_include_directories(snake_viewer PUBLIC SDL2::SDL2)
target_link_libraries(snake_gui PRIVATE SDL2::SDL2  SDL2::SDL2main gamelib nnlib)
