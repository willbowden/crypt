#include "main.h"

void cleanup_game(Game *game)
{
  if (!game) return;

  if (game->graphics)
  {
    cleanup_graphics(game->graphics);
  }

  free(game);
}

int main(int argc, char **argv)
{
  Game *game = (Game *)calloc(1, sizeof(Game));
  int running = 1;
  SDL_Event e;
  
  game->graphics = initialise_graphics();

  if (!game->graphics)
  {
    printf("%s\n", "Error initialising graphics engine");
    cleanup_game(game);
    return 1;
  }


  while (running)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        running = 0;
      }
    }

    game->graphics->render_frame(game->graphics);
  }

  cleanup_game(game);

  return 0;
}
