#include "nn/neural_network.h"
#include "game/game.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <SDL2/SDL.h>

const int GRID_SIZE = 10;
const int CELL_SIZE = 50;
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE;
void render_grid(SDL_Renderer* renderer, const std::vector<std::vector<int>>& map) {
  for (int y = 0; y < GRID_SIZE; ++y) {
    for (int x = 0; x < GRID_SIZE; ++x) {
      SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
      if (map[y][x] == 1) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
      } else if (map[y][x] == 2) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
      } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
      }
      SDL_RenderFillRect(renderer, &cell);
      // Draw grid lines
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &cell);
    }
  }
}

Direction get_direction(Game& game, MLP& mlp) {
  std::vector<float> features = game.get_features();
  std::vector<float> output = mlp.forward(features);
  int max_idx = 0;
  for (int i = 1; i < output.size(); i++) {
    if (output[i] > output[max_idx]) max_idx = i;
  }
  return static_cast<Direction>(max_idx + 1);
}

int main() {
  // currently, a pre-trained model is places in the root directory of the repo
  SDL_SetHint(SDL_HINT_VIDEO_X11_FORCE_EGL, "0");
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("Snake AI Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  std::string model_name = "./demo_1000_1000";
  MLP mlp(model_name);
  std::random_device rd;
  std::mt19937 rng(rd());
  Game game = Game(rng);
  Direction dir;
  std::vector<std::vector<int> > board(10, std::vector<int>(10, 0)); 
  do {
    // game.dump();
    printf("Direction: %d\n", dir);
    dir = get_direction(game, mlp);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    game.get_game_state(board);
    SDL_RenderPresent(renderer);
    render_grid(renderer, board);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  } while (game.run(dir));
  std::cout << game.calculate_fitness() << std::endl;
}