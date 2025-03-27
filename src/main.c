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

int save_game(Game *game, const char *levelName, const char *saveFilename) {
  int x, y;
  SaveData data;
  FILE *file = fopen(saveFilename, "wb");

  if (file == NULL) {
    fprintf(stderr, "Error: Could not create/open the save file");
    return 1;
  }
  data.playerX = game->player->worldX;
  data.playerY = game->player->worldY;
  data.playerHealth = game->player->health;
  strncpy(data.levelName, levelName, strlen(levelName));
  data.levelName[strlen(levelName)] = '\0';

  for(y = 0; y < WINDOW_HEIGHT_SPRITES; y++) {
    for(x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
      if(game->level->foreground[y][x] == NULL) {
        data.foregroundGrid[y][x].type = INVALID;
      } else {
        data.foregroundGrid[y][x] = *(game->level->foreground[y][x]);
      }
    }
  }
  fwrite(&data, sizeof(SaveData), 1, file);
  fclose(file);
  return 0;
}

/* TODO: modify add_player function such that it accepts the player as an input and returns an int as its output.
It doesn't create a player in that function, instead uses the player that is passed as an argument*/
Game *load_game(const char *saveFilename){
  int x, y;
  SaveData loadData;
  Player *player;
  FILE *file = fopen(saveFilename, "rb");
  Game *game = (Game *) malloc(sizeof(Game));

  if(game == NULL) {
    fprintf(stderr, "Error: Could not initiaze game");
  }

  if(file == NULL) {
    fprintf(stderr, "Error: Could not open the save file");
    return NULL;
  }

  fread(&loadData, sizeof(SaveData), 1, file);
  player = create_player(sprite_from_number(25), loadData.playerHealth, loadData.playerX, loadData.playerY);

  if(player == NULL) {
    fprintf(stderr, "Error: Could not create player");
    return NULL;
  }
  
  if (initialise_game(game, loadData.levelName, player)) {
    fprintf(stderr, "Error: Failed to initialize game.");
    return NULL;
  }

  for(y = 0; y < WINDOW_HEIGHT_SPRITES; y++) {
    for(x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
      if(loadData.foregroundGrid[y][x].type == INVALID) {
        game->level->foreground[y][x] = NULL;
      } else {
      *(game->level->foreground[y][x]) = loadData.foregroundGrid[y][x];
      }
    }
  }

  fclose(file);
  return game;
}

void handle_keypress(Game *game, SDL_Event *e)
{
  SDL_KeyCode key = e->key.keysym.sym;

  if (key >= SDLK_RIGHT && key <= SDLK_UP)
  {
    move_player(game, key);
  }
}

int initialise_game(Game *game, char *levelName, Player *player)
{  
  game->graphics = initialise_graphics();

  if (!game->graphics)
  {
    fprintf(stderr, "%s\n", "Error initialising graphics engine");
    cleanup_game(game);
    return 1;
  }

  printf("%s\n", levelName);
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

/* Saving on quit and saving periodically between turns */
void run_game(Game *game)
{
  int running = 1;
  SDL_Event e;
  Game *game = load_game("./saves/save1");
  char *levelName = "./assets/Levels/Level1";
  
  game->state = LOADING;
  
  if(game == NULL) {
    fprintf(stderr, "Error: Unable to initialize game");
    return 1;
  }
  
  add_player(game);


  /*if(player == NULL) {
    fprintf(stderr, "Error: Something went wrong when creating the player.");
    return 1;
  }

  if (initialise_game(game, levelName, player) != 0)
  {
    return 1;
  }*/

  draw_level(game->graphics, game->level);
  present_frame(game->graphics);

  game->state = PLAYER_TURN;
  run_game(game);


  if(save_game(game, levelName, "./saves/save1")) {
    fprintf(stderr, "Error: Something went wrong while saving the game");
    return 1;
  }
  printf("Game has been saved!");
  cleanup_game(game);

  return 0;
}
