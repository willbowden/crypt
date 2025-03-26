#include "main.h"

void cleanup_game(Game *game)
{
  if (!game)
    return;

  if (game->graphics)
  {
    cleanup_graphics(game->graphics);
  }

  if (game->level)
  {
    cleanup_level(game->level);
  }

  if (game->player)
  {
    free_player(game->player);
  }

  free(game);
}

int initialise_game(Game *game)
{
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

  game->player = add_player(game, 25, 29, 9);

  if (!game->level)
  {
    fprintf(stderr, "%s\n", "Error adding player");
    cleanup_game(game);
    return 1;
  }

  return 0;
}

void handle_keypress(Game *game, SDL_Event *e)
{
  SDL_KeyCode key = e->key.keysym.sym;

  switch (game->state)
  {
  case PLAYER_TURN:
    if (key >= SDLK_RIGHT && key <= SDLK_UP)
    {
      move_player(game, key);
      game->state = ENEMY_TURN;
    } else if (key == SDLK_p) {
      game->state = PAUSED;
    }
    break;
  case PAUSED:
    if (key == SDLK_p)
    {
      game->state = PLAYER_TURN;
    }
    break;
  case MENU_OPEN:
    /**
     * TODO: Handle menu inputs here
     */
    break;
  case DIALOG_OPEN:
    /**
     * TODO: Handle dialog inputs here
     */
    break;
  default:
    break;
  }
}

void run_game(Game *game)
{
  int running = 1;
  SDL_Event e;

  while (running)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        running = 0;
      }

      switch (game->state)
      {
      case ENEMY_TURN:
        /** TODO: Process enemy turns here */
        game->state = PLAYER_TURN;
        break;
      case LOADING:
        break;
      /**
       * If it's neither the enemy's turn nor is the game loading,
       *  the player's key input must be handled.
       */
      default:
        if (e.type == SDL_KEYUP)
        {
          handle_keypress(game, &e);
          clear_screen(game->graphics);
          draw_level(game->graphics, game->level);
          present_frame(game->graphics);
        }
        break;
      }
    }
  }
}

int main(int argc, char **argv)
{
  Game *game = (Game *)calloc(1, sizeof(Game));
  game->state = LOADING;

  if (initialise_game(game) != 0)
  {
    return 1;
  }

  game->state = PLAYER_TURN;

  draw_level(game->graphics, game->level);
  present_frame(game->graphics);

  run_game(game);

  cleanup_game(game);

  return 0;
}
