#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

#define SQUARE_SIDE 200.00
#define ALPHA 0.03
#define BETA 0.02
#define GAMMA 0.02

struct Coordinates {
  double x;
  double y;
  double z;
};

void initialize_cube(struct Coordinates *square, SDL_Event event) {
  int points_per_edge = SQUARE_SIDE / 1;
  for (int i = 0; i < points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2 + i,
                 (double)event.motion.y - SQUARE_SIDE / 2, SQUARE_SIDE / 2};
  }
  for (int i = points_per_edge; i < 2 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2 + i - 200.00,
                 (double)event.motion.y + SQUARE_SIDE / 2, SQUARE_SIDE / 2};
  }
  for (int i = 2 * points_per_edge; i < 3 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2 + i - 400.00,
                 (double)event.motion.y - SQUARE_SIDE / 2, -SQUARE_SIDE / 2};
  }
  for (int i = 3 * points_per_edge; i < 4 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2 + i - 600.00,
                 (double)event.motion.y + SQUARE_SIDE / 2, -SQUARE_SIDE / 2};
  }
  for (int i = 4 * points_per_edge; i < 5 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2,
                 (double)event.motion.y - SQUARE_SIDE / 2 + i - 800.00,
                 SQUARE_SIDE / 2};
  }
  for (int i = 5 * points_per_edge; i < 6 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x + SQUARE_SIDE / 2,
                 (double)event.motion.y - SQUARE_SIDE / 2 + i - 1000.00,
                 SQUARE_SIDE / 2};
  }
  for (int i = 6 * points_per_edge; i < 7 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2,
                 (double)event.motion.y - SQUARE_SIDE / 2 + i - 1200.00,
                 -SQUARE_SIDE / 2};
  }
  for (int i = 7 * points_per_edge; i < 8 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x + SQUARE_SIDE / 2,
                 (double)event.motion.y - SQUARE_SIDE / 2 + i - 1400.00,
                 -SQUARE_SIDE / 2};
  }
  for (int i = 8 * points_per_edge; i < 9 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x + SQUARE_SIDE / 2,
                 (double)event.motion.y + SQUARE_SIDE / 2,
                 -SQUARE_SIDE / 2 + i - 1600.00};
  }
  for (int i = 9 * points_per_edge; i < 10 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2,
                 (double)event.motion.y - SQUARE_SIDE / 2,
                 -SQUARE_SIDE / 2 + i - 1800.00};
  }
  for (int i = 10 * points_per_edge; i < 11 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x - SQUARE_SIDE / 2,
                 (double)event.motion.y + SQUARE_SIDE / 2,
                 -SQUARE_SIDE / 2 + i - 2000.00};
  }
  for (int i = 11 * points_per_edge; i < 12 * points_per_edge; i++) {
    square[i] = {(double)event.motion.x + SQUARE_SIDE / 2,
                 (double)event.motion.y - SQUARE_SIDE / 2,
                 -SQUARE_SIDE / 2 + i - 2200.00};
  }
}

void draw_points_3d(struct Coordinates *square, SDL_Renderer *renderer,
                    SDL_Event event) {
  int x, y;
  for (int i = 0; i < 12 * (int)SQUARE_SIDE; i++) {
    SDL_Rect rect = {
        (int)square[i].x + event.motion.x,
        (int)square[i].y + event.motion.y,
        1,
        1,
    };
    SDL_SetRenderDrawColor(renderer, 40, 240, 240, 240);
    SDL_RenderDrawRect(renderer, &rect);
  }
}

void rotateShape(struct Coordinates *square) {
  double rotation_matrix[3][3] = {
      {cos(BETA) * cos(GAMMA), -cos(BETA) * sin(GAMMA), sin(BETA)},
      {cos(ALPHA) * sin(GAMMA) + sin(ALPHA) * sin(BETA) * cos(GAMMA),
       cos(ALPHA) * cos(GAMMA) - sin(ALPHA) * sin(BETA) * sin(GAMMA),
       -sin(ALPHA) * cos(BETA)},
      {sin(ALPHA) * sin(GAMMA) - cos(ALPHA) * sin(BETA) * cos(GAMMA),
       sin(ALPHA) * cos(GAMMA) + cos(ALPHA) * sin(BETA) * sin(GAMMA),
       cos(ALPHA) * cos(BETA)}};
  for (int i = 0; i < 12 * SQUARE_SIDE; i++) {

    double x = square[i].x;
    double y = square[i].y;
    double z = square[i].z;

    square[i].x = rotation_matrix[0][0] * x + rotation_matrix[0][1] * y +
                  rotation_matrix[0][2] * z;
    square[i].y = rotation_matrix[1][0] * x + rotation_matrix[1][1] * y +
                  rotation_matrix[1][2] * z;
    square[i].z = rotation_matrix[2][0] * x + rotation_matrix[2][1] * y +
                  rotation_matrix[2][2] * z;
  }
}

int main(int argc, char **argv) {
  if (!SDL_Init(SDL_INIT_EVERYTHING)) {
    std::cout << "SDL initialized" << std::endl;
  }
  SDL_Window *window = SDL_CreateWindow("Rotating Cube", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 600,
                                        SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Event event;
  bool status = true;

  struct Coordinates square[12 * (unsigned int)SQUARE_SIDE];
  initialize_cube(square, event);

  while (status) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      status = false;
    }

    SDL_RenderClear(renderer);
    rotateShape(square);
    draw_points_3d(square, renderer, event);
    SDL_SetRenderDrawColor(renderer, 40, 40, 80, 40);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  std::cout << "closing window" << std::endl;

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
