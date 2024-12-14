#include "SDL_error.h"
#include "game/util.h"
#include "nn/neural_network.h"
#include "game/game.h"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

const int GRID_SIZE = 10;
const int CELL_SIZE = 50;
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE;
void render_grid(SDL_Renderer* renderer, const std::vector<std::vector<int>>& map) {
  for (int y = 0; y < GRID_SIZE; ++y) {
    for (int x = 0; x < GRID_SIZE; ++x) {
      SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
      
      switch (map[y][x]) {
        case 1:
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
          break;
        case 2:
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
          break;
        default:
          SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray
          break;
      }

      // Fill the cell
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

int main(int argc, char *argv[]) {
  // currently, a pre-trained model is places in the root directory of the repo
  std::string model_name = "./demo_1000_1000";
  MLP mlp(model_name);
  std::random_device rd;
  std::mt19937 rng(rd());
  Game game = Game(rng);
  Direction dir = None;
  std::vector<std::vector<int> > board(10, std::vector<int>(10, 0)); 

  // 初始化 SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
      return 1;
  }

  // 創建視窗
  SDL_Window* window = SDL_CreateWindow(
      "Sanke Ai Viewer",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_SIZE, WINDOW_SIZE,
      SDL_WINDOW_SHOWN
  );

  if (!window) {
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return 1;
  }


  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
      std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(window);
      SDL_Quit();
      return 1;
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  bool running = true;
  SDL_Event event;
  do {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    game.get_game_state(board);
    render_grid(renderer, board);

    SDL_RenderPresent(renderer);

    SDL_Delay(50); 
    dir = get_direction(game, mlp);
  } while (game.run(dir) && running);
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << game.calculate_fitness() << std::endl;
}