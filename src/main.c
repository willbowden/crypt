#include "main.h"

void cleanup_game(Game *game)
{
  if (!game) return;

  if (game->graphics)
  {
    cleanup_graphics(game->graphics);
  }

  if (game->level)
  {
    cleanup_level(game->level);
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
    fprintf(stderr, "%s\n", "Error initialising graphics engine");
    cleanup_game(game);
    return 1;
  }

  game->level = load_level("./assets/Levels/Level1");

  if (!game->level)
  {
    fprintf(stderr, "%s\n", "Error loading level");
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

    /* Render loop, can put steps in here for UI or player movement*/
    clear_screen(game->graphics);
    draw_level(game->graphics, game->level);
    present_frame(game->graphics);
  }

  cleanup_game(game);

  return 0;
}
