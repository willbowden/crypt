#include "main.h"
#include "enemy.h"
#include "entity_system.h"
#include <sys/stat.h>
#include <sys/types.h>

void cleanup_game(Game *game)
{
  if (!game)
    return;

  if (game->graphics)
  {
    cleanup_graphics(game->graphics);
  }

  if (game->menu)
  {
    cleanup_menu(game->menu);
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

void create_directories(const char *file_path)
{
  char *dir_path = (char *)malloc(strlen(file_path) + 1);
  char *next_sep = strchr(file_path, '/');
  while (next_sep != NULL)
  {
    int dir_path_len = next_sep - file_path;
    memcpy(dir_path, file_path, dir_path_len);
    dir_path[dir_path_len] = '\0';
    mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH);
    next_sep = strchr(next_sep + 1, PATH_SEPARATOR);
  }
  free(dir_path);
}

Game *initialise_game()
{
  Game *game = (Game *)malloc(sizeof(Game));

  game->graphics = initialise_graphics();

  if (!game->graphics)
  {
    fprintf(stderr, "%s\n", "Error initialising graphics engine");
    cleanup_game(game);
    return NULL;
  }

  game->menu = initialise_menu();

  if (!game->menu)
  {
    cleanup_game(game);
    fprintf(stderr, "%s\n", "Error initialising Menu State");
    return NULL;
  }

  game->state = LOADING;

  return game;
}

/**
 * Create a new game from level 1
 */
int new_game(Game *game)
{
  if (game == NULL)
  {
    return -1;
  }

  game->level = load_level(1, 0);

  if (!game->level)
  {
    fprintf(stderr, "%s\n", "Error loading level");
    cleanup_game(game);
    return -1;
  }

  game->player = create_player(sprite_from_number(25), 100, 29, 5);

  if (!game->player)
  {
    fprintf(stderr, "%s\n", "Error adding player");
    cleanup_game(game);
    return -1;
  }

  add_player(game);

  spawn_random_enemies(game, GOBLIN, 4);

  game->state = PLAYER_TURN;
  return 0;
}

/**
 * Load game from save file
 * Currently a fixed single file
 */
int load_game(Game *game)
{
  char saveFilename[] = "./saves/save1";
  int i;
  SaveHeader header;
  ForegroundDataHeader data;
  Player *player = (Player *)malloc(sizeof(Player));
  Enemy *enemy;
  FILE *file = fopen(saveFilename, "rb");
  game->state = LOADING;

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not open the save file. Starting a new game.");
    return new_game(game);
  }

  fread(&header, sizeof(SaveHeader), 1, file);
  player->sprite = sprite_from_number(header.playerSpriteNo);
  player->type = PLAYER;
  player->attack = header.playerAttack;
  player->defense = header.playerDefense;
  player->health = header.playerHealth;
  player->worldX = header.playerWorldX;
  player->worldY = header.playerWorldY;
  
  game->player = player;

  game->level = load_level(header.levelNumber, 1);
  if (game->level == NULL)
  {
    fprintf(stderr, "%s\n", "Error initialising level from save");
    return -1;
  }
  game->level->levelState = header.levelState;
  
  add_player(game);
  
  /* Initialize the Entities in the foreground */
  for (i = 0; i < header.numEntities; i++)
  {
    fread(&data, sizeof(ForegroundDataHeader), 1, file);
    
    switch (data.entityType)
    {
      case ENEMY:
      enemy = (Enemy *) malloc(sizeof(Enemy));
      fread(enemy, sizeof(Enemy), 1, file);
      enemy->sprite = sprite_from_number(data.tileNo);
      game->level->foreground[data.y][data.x] = (Entity *)enemy;
      game->level->enemyCount++;
      break;
      default:
      game->level->foreground[data.y][data.x] = entity_from_number(data.tileNo);
      break;
    }
  }
  
  fclose(file);

  game->state = PLAYER_TURN;

  return 0;
}

/**
 * Save game to file
 * Currently a fixed single file
 */
int save_game(Game *game)
{
  char saveFilename[] = "./saves/save1";
  int x, y, entityCount;
  FILE *file;
  ForegroundDataHeader data;
  SaveHeader header;
  GameState previousState = game->state;
  game->state = LOADING;

  /**
   * If there's no game state to save
   *  (i.e user has opened game but hasn't used main menu),
   *  cancel the save
   */
  if (game->player == NULL || game->level == NULL)
  {
    return 1;
  }

  create_directories(saveFilename);

  file = fopen(saveFilename, "wb");

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not create/open the save file");
    return -1;
  }


  header.playerWorldX = game->player->worldX;
  header.playerWorldY = game->player->worldY;
  header.playerAttack = game->player->attack;
  header.playerDefense = game->player->defense;
  header.playerSpriteNo = game->player->sprite->tileNo;
  header.playerHealth = game->player->health;
  header.levelNumber = game->level->levelNumber;
  header.levelState = game->level->levelState;

  entityCount = 0;
  for(y = 0; y < WORLD_HEIGHT_SPRITES; y++){
    for(x = 0; x < WORLD_WIDTH_SPRITES; x++) {
      if(game->level->foreground[y][x] != NULL) {
        entityCount++;
      }
    }
  }

  header.numEntities = entityCount;

  fwrite(&header, sizeof(SaveHeader), 1, file);

  for (y = 0; y < WORLD_HEIGHT_SPRITES; y++)
  {
    for (x = 0; x < WORLD_WIDTH_SPRITES; x++)
    {
      if (game->level->foreground[y][x] != NULL)
      {
        data.x = x;
        data.y = y;
        data.tileNo = game->level->foreground[y][x]->sprite->tileNo;
        data.entityType = game->level->foreground[y][x]->type;
        fwrite(&data, sizeof(ForegroundDataHeader), 1, file);

        /* Player data not considered since it is already stored in the header. */
        switch (game->level->foreground[y][x]->type)
        {
        case ENEMY:
          fwrite((Enemy *)game->level->foreground[y][x], sizeof(Enemy), 1, file);
          break;
        default:
          break;
        }
      }
    }
  }
  fclose(file);

  game->state = previousState;
  return 0;
}

void quit_game(Game *game)
{
  cleanup_game(game);
  exit(0);
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
    }
    else if (key == SDLK_p)
    {
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
    if (key == SDLK_UP)
    {
      if (game->menu->selected_item > 0)
      {
        game->menu->selected_item--;
      } else {
        game->menu->selected_item = game->menu->num_items - 1;
      }
    } else if (key == SDLK_DOWN)
    {
      if (game->menu->selected_item < game->menu->num_items - 1)
      {
        game->menu->selected_item++;
      } else {
        game->menu->selected_item = 0;
      }
    } else if (key == SDLK_RETURN)
    {
      if(game->menu->menu_items[game->menu->selected_item].select(game) != 0)
      {
        fprintf(stderr, "Error selecting menu item with text: %s\n", game->menu->menu_items[game->menu->selected_item].text);
        exit(-1);
      }
    }
    break;
  case DIALOG_OPEN:
    game->state = PLAYER_TURN;
    dismiss_popup(game);
    break;
  default:
    break;
  }
}

/**
 * Main game loop with fixed-FPS rendering and input handling
 */
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
          enemy_turn(game); /* Process enemy movements */
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
            if (game->state == PLAYER_TURN)
            {
              turns++;
            }
            handle_keypress(game, &e);
          }
          break;
        }

        if (turns % SAVE_INTERVAL == 0)
        {
          /* Make sure to replace the levelName and saveFile name with their respective variables to make it modular. */
          if (save_game(game) == -1)
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
  Game *game = initialise_game();
  
  if (game == NULL)
  {
    fprintf(stderr, "Error: Unable to initialize game");
    return 1;
  }

  game->state = MENU_OPEN;
  render(game);
  run_game(game);

  if (save_game(game) == -1)
  {
    fprintf(stderr, "Error: Something went wrong while saving the game\n");
  }
  cleanup_game(game);

  return 0;
}
