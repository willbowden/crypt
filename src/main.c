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

  game->ui = initialize_ui();

  if (!game->ui)
  {
    cleanup_game(game);
    fprintf(stderr, "%s\n", "Error initialising UI");
    return NULL;
  }

  return game;
}

Game *new_game()
{
  Game *game = initialise_game();
  if (game == NULL)
  {
    return NULL;
  }

  game->level = load_level(1, 0);

  if (!game->level)
  {
    fprintf(stderr, "%s\n", "Error loading level");
    cleanup_game(game);
    return NULL;
  }

  game->player = create_player(sprite_from_number(25), 100, 29, 5);

  if (!game->player)
  {
    fprintf(stderr, "%s\n", "Error adding player");
    cleanup_game(game);
    return NULL;
  }

  add_player(game);

  return game;
}

Game *load_game(const char *saveFilename)
{
  int i;
  SaveHeader header;
  ForegroundDataHeader data;
  Player *player = (Player *)malloc(sizeof(Player));
  Enemy *enemy;
  FILE *file = fopen(saveFilename, "rb");
  Game *game;

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not open the save file. Starting a new game.");
    return new_game();
  }

  game = initialise_game();

  fread(&header, sizeof(SaveHeader), 1, file);
  player->sprite = sprite_from_number(header.playerSpriteNo);
  player->type = PLAYER;
  player->attack = header.playerAttack;
  player->defense = header.playerDefense;
  player->health = header.playerHealth;
  player->worldX = header.playerWorldX;
  player->worldY = header.playerWorldY;
  game->state = PLAYER_TURN;

  game->player = player;

  game->level = load_level(header.levelNumber, 1);
  if (game->level == NULL)
  {
    fprintf(stderr, "%s\n", "Error initialising level from save");
    return NULL;
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

  return game;
}

int save_game(Game *game, const char *saveFilename)
{
  int x, y, entityCount;
  FILE *file;
  ForegroundDataHeader data;
  SaveHeader header;

  create_directories(saveFilename);

  file = fopen(saveFilename, "wb");

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not create/open the save file");
    return 1;
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
          enemy_turn(game); /* Process enemy movements */
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
          if (save_game(game, "./saves/save1"))
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

int compute_next_move(Game *game, Enemy *enemy, int *nextX, int *nextY)
{
  int gridW = WORLD_WIDTH_SPRITES;
  int gridH = WORLD_HEIGHT_SPRITES;
  int bestDist = 9999;
  int chosenX = enemy->worldX;
  int chosenY = enemy->worldY;
  int dx[4] = {0, 1, 0, -1};
  int dy[4] = {1, 0, -1, 0};
  int i;
  int dist;

  for (i = 0; i < 4; i++)
  {
    int nx = enemy->worldX + dx[i];
    int ny = enemy->worldY + dy[i];
    if (nx < 0 || nx >= gridW || ny < 0 || ny >= gridH)
      continue;
    /* Allow movement if the cell is empty or occupied by the player */
    if (game->level->foreground[ny][nx] != NULL &&
        game->level->foreground[ny][nx]->type != PLAYER)
      continue;
    /* Compute Manhattan distance from candidate cell to player's position */
    dist = abs(game->player->worldX - nx) + abs(game->player->worldY - ny);
    if (dist < bestDist)
    {
      bestDist = dist;
      chosenX = nx;
      chosenY = ny;
    }
  }

  /* If no neighbor found (should not happen in normal gameplay), return 0 */
  if (chosenX == enemy->worldX && chosenY == enemy->worldY)
    return 0;

  *nextX = chosenX;
  *nextY = chosenY;
  return 1;
}

/* Saving on quit and saving periodically between turns */
int main()
{
  Game *game = load_game("./saves/save1");

  if (game == NULL)
  {
    fprintf(stderr, "Error: Unable to initialize game");
    return 1;
  }

  if (game->state == LOADING)
  {
    spawn_random_enemies(game, 4);
  }

  render(game);
  game->state = PLAYER_TURN;
  run_game(game);

  if (save_game(game, "./saves/save1"))
  {
    fprintf(stderr, "Error: Something went wrong while saving the game\n");
    return 1;
  }
  printf("Game has been saved!\n");
  cleanup_game(game);

  return 0;
}
