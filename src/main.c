#include "main.h"

void cleanup_game(Game *game)
{
  if (!game)
    return;

  if (game->graphics)
  {
    cleanup_graphics(game->graphics);
  }

  if (game->ui)
  {
    cleanup_ui(game->ui);
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

/* TODO: Save with level completed function is implemented*/
int save_game(Game *game, const char *levelName, const char *saveFilename)
{
  int x, y;
  SaveData data;
  FILE *file = fopen(saveFilename, "wb");

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not create/open the save file");
    return 1;
  }
  data.playerX = game->player->worldX;
  data.playerY = game->player->worldY;
  data.playerHealth = game->player->health;
  strncpy(data.levelName, levelName, strlen(levelName));
  data.levelName[strlen(levelName)] = '\0';

  for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
  {
    for (x = 0; x < WINDOW_WIDTH_SPRITES; x++)
    {
      if (game->level->foreground[y][x] == NULL)
      {
        data.foregroundGrid[y][x].type = INVALID;
      }
      else
      {
        data.foregroundGrid[y][x] = *(game->level->foreground[y][x]);
      }
    }
  }
  fwrite(&data, sizeof(SaveData), 1, file);
  fclose(file);
  return 0;
}

Game *load_game(const char *saveFilename)
{
  int x, y;
  int found = 1; /* Variable to check if the load_game function found the save file */
  SaveData loadData;
  Player *player;
  FILE *file = fopen(saveFilename, "rb");
  Game *game = (Game *)malloc(sizeof(Game));

  if (game == NULL)
  {
    fprintf(stderr, "Error: Could not initiaze game");
    return NULL;
  }

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not open the save file. Starting a new game.");
    game->state = LOADING;
    found = 0;
    loadData.playerX = 29;
    loadData.playerY = 5;
    loadData.playerHealth = 100;
    strcpy(loadData.levelName, "./assets/Levels/Level1");
  }
  else
  {
    fread(&loadData, sizeof(SaveData), 1, file);
  }

  player = create_player(sprite_from_number(25), loadData.playerHealth, loadData.playerX, loadData.playerY);

  if (player == NULL)
  {
    fprintf(stderr, "Error: Could not create player");
    return NULL;
  }

  if (initialise_game(game, loadData.levelName, player))
  {
    fprintf(stderr, "Error: Failed to initialize game.");
    return NULL;
  }

  add_player(game, loadData.playerX, loadData.playerY);

  if (found)
  {
    for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
    {
      for (x = 0; x < WINDOW_WIDTH_SPRITES; x++)
      {
        if (loadData.foregroundGrid[y][x].type == INVALID)
        {
          game->level->foreground[y][x] = NULL;
        }
        else
        {
          if (game->level->foreground[y][x] == NULL)
          {
            game->level->foreground[y][x] = (Entity *)malloc(sizeof(Entity));
          }

          if (game->level->foreground[y][x] == NULL)
          {
            fprintf(stderr, "Error: Could not allocate memory for foreground entity\n");
            return NULL;
          }

          *(game->level->foreground[y][x]) = loadData.foregroundGrid[y][x];
        }
      }
    }
    fclose(file);
  }
  return game;
}

int initialise_game(Game *game, char *levelName, Player *player)
{
  game->graphics = initialise_graphics();
  game->ui = initialize_ui();

  if (!game->graphics)
  {
    fprintf(stderr, "%s\n", "Error initialising graphics engine");
    cleanup_game(game);
    return 1;
  }

  if (!game->ui)
  {
    cleanup_game(game);
    fprintf(stderr, "%s\n", "Error initialising UI");
    return 1;
  }

  game->level = load_level(levelName);

  if (!game->level)
  {
    fprintf(stderr, "%s\n", "Error loading level");
    cleanup_game(game);
    return 1;
  }

  game->player = player;

  if (!game->player)
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
    }
    else if (key == SDLK_p)
    {
      game->state = PAUSED;
    }
    else if (key == SDLK_o)
    {
      add_animation(
          game->graphics,
          &game->player->worldX,
          &game->player->worldY,
          game->player->sprite,
          GAME_FPS/4,
          &flashing_red_animation);
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
    game->state = PLAYER_TURN;
    dismiss_popup(game);
    break;
  default:
    break;
  }
}

void run_game(Game *game)
{
  int running = 1;
  int turns = 1;
  SDL_Event e;
  float a, b, delta;

  while (running)
  {
    a = SDL_GetTicks();
    delta = a - b;

    if (delta > 1000.0 / GAME_FPS)
    {
      b = a;  
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
          if (e.type == SDL_KEYDOWN)
          {
            if (game->state == PLAYER_TURN)
            {
              turns++;
            }
            handle_keypress(game, &e);
          }
          break;
        }

        if (turns % 5 == 0)
        {
          /* Make sure to replace the levelName and saveFile name with their respective variables to make it modular. */
          if (save_game(game, "./assets/Levels/Level1", "./saves/save1"))
          { /* Save every 5 turns */
            fprintf(stderr, "Error: Something went wrong while saving the game");
            return;
          }
          turns = 0;
        }
      }
      render(game);
    }
  }
}

/* Saving on quit and saving periodically between turns */
int main()
{
  Game *game = load_game("./saves/save1");
  char *levelName = "./assets/Levels/Level1";

  game->state = LOADING;

  if (game == NULL)
  {
    fprintf(stderr, "Error: Unable to initialize game");
    return 1;
  }

  render(game);

  game->state = PLAYER_TURN;
  run_game(game);

  if (save_game(game, levelName, "./saves/save1"))
  {
    fprintf(stderr, "Error: Something went wrong while saving the game");
    return 1;
  }
  printf("Game has been saved!");
  cleanup_game(game);

  return 0;
}
