#include "main.h"
#include "enemy.h"
#include "entity_system.h"
#include <sys/stat.h>
#include <sys/types.h>
#define pathSeparator '/'

/* Plan: 
- First make a header and store it
- Then save the foreground data
- When you load this foreground data, go through all of the items in the grid and if there doesn't exist an element in that x, y coordinate, give it null*/

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

void create_directories(const char *file_path) {
  char *dir_path = (char *) malloc(strlen(file_path) + 1);
  char *next_sep = strchr(file_path, '/');
  while (next_sep != NULL) {
    int dir_path_len = next_sep - file_path;
    memcpy(dir_path, file_path, dir_path_len);
    dir_path[dir_path_len] = '\0';
    mkdir(dir_path, S_IRWXU|S_IRWXG|S_IROTH);
    next_sep = strchr(next_sep + 1, pathSeparator);
  }
  free(dir_path);
}

/* TODO: Save with level completed function is implemented*/
int save_game(Game *game, int levelNumber, const char *saveFilename)
{
  int x, y;
  int entityCount;
  SaveHeader header;
  FILE *file;
  ForegroundDataHeader data;
  InteractableData interactableData;
  EnemyData enemyData;
  Enemy *enemy;
  Interactable *interactable;
  
  create_directories(saveFilename);
  
  file = fopen(saveFilename, "wb");

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not create/open the save file");
    return 1;
  }
  header.player = *(game->player);
  header.levelNumber = levelNumber;
  header.levelState = game->level->levelState;

  entityCount = 0;
  for(y = 0; y < WINDOW_HEIGHT_SPRITES; y++){
    for(x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
      if(game->level->foreground[y][x] != NULL) {
        entityCount++;
      }
    }
  }

  header.numEntities = entityCount;
  fwrite(&header, sizeof(SaveHeader), 1, file);

  for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
  {
    for (x = 0; x < WINDOW_WIDTH_SPRITES; x++)
    {
      if(game->level->foreground[y][x] != NULL) {
        data.x = x;
        data.y = y;
        data.type = game->level->foreground[y][x]->type;
        fwrite(&data, sizeof(ForegroundDataHeader), 1, file);

        /* Player data not considered since it is already stored in the header. */
        switch (game->level->foreground[y][x]->type)
        {
        case ENEMY:
          enemy = (Enemy *) game->level->foreground[y][x];
          enemyData.enemyType = enemy->enemy_type;
          enemyData.health = enemy->health;
          fwrite(&enemyData, sizeof(EnemyData), 1, file);
          break;
        case INTERACTABLE:
          interactable = (Interactable *) game->level->foreground[y][x];
          interactableData.funcId = interactable->funcId;
          interactableData.type = interactable->interactableType;
          fwrite(&interactableData, sizeof(InteractableData), 1, file);
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

Game *load_game(const char *saveFilename)
{
  int x, y, i;
  int found = 1; /* Variable to check if the load_game function found the save file */
  SaveHeader header;
  ForegroundDataHeader data;
  InteractableData interactableData;
  EnemyData enemyData;
  Player *player;
  Enemy *enemy;
  Interactable *interactable;
  FILE *file = fopen(saveFilename, "rb");
  Game *game = (Game *)malloc(sizeof(Game));

  if (game == NULL)
  {
    fprintf(stderr, "Error: Could not initiaze game");
    return NULL;
  }

  printf("The game has been created");
  player = create_player(sprite_from_number(25), 100, 29, 5); 
  if (player == NULL)
  {
    fprintf(stderr, "Error: Could not create player");
    return NULL;
  }

  printf("The playuer has been created");

  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not open the save file. Starting a new game.");
    game->state = LOADING;
    found = 0;
    header.levelNumber = 1;
    header.levelState = create_empty_level_state();
  }
  else
  {
    fread(&header, sizeof(SaveHeader), 1, file);
    *player = header.player;
    game->state = PLAYER_TURN;
    printf("The save header file has been loaded");
  }

  if (initialise_game(game, header.levelNumber, player))
  {
    fprintf(stderr, "Error: Failed to initialize game.");
    return NULL;
  }

  printf("Game initialized");
  add_player(game, player->worldX, player->worldY);

  /* Initialize the Entities in the foreground */
  if (found)
  {
    /* Resetting all interactables to NULL for accurate reload later */
    for(y = 0; y < WINDOW_HEIGHT_SPRITES; y++) {
      for(x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
        if(game->level->foreground[y][x] != NULL && game->level->foreground[y][x]->type == INTERACTABLE) {
          game->level->foreground[y][x] = NULL; 
        }
      }
    }

    for(i = 0; i < header.numEntities; i++) {
      fread(&data, sizeof(ForegroundDataHeader), 1, file);

      switch (data.type)
      {
      case ENEMY:
        fread(&enemyData, sizeof(EnemyData), 1, file);
        enemy = add_enemy(game, enemyData.enemyType);
        if(enemy == NULL) {
          fprintf(stderr, "Error: Could not create enemy from save data\n");
        }
        enemy->health = enemyData.health;
        enemy->hasMoved = 0;
        enemy->worldX = data.x;
        enemy->worldY = data.y;
        game->level->foreground[data.y][data.x] = (Entity *) enemy;
        break;
      case INTERACTABLE:
        fread(&interactableData, sizeof(InteractableData), 1, file);
        printf("This is the interactable tile no: %d", interactableData.type);
        interactable = create_interactable(sprite_from_number(interactableData.type), interactableData.funcId, interactableData.type);
        game->level->foreground[data.y][data.x] = (Entity *) interactable;
        break;
      default:
        break;
      }
    }
    fclose(file);
  }
  return game;
}

int initialise_game(Game *game, int levelNumber, Player *player)
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

  game->level = load_level(levelNumber);

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
        enemy_turn(game);  /* Process enemy movements */
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
          if (save_game(game, game->level->levelNumber, "./saves/save1"))
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

int compute_next_move(Game *game, Enemy *enemy, int *nextX, int *nextY) {
  int gridW = WINDOW_WIDTH_SPRITES;
  int gridH = WINDOW_HEIGHT_SPRITES;
  int bestDist = 9999;
  int chosenX = enemy->worldX;
  int chosenY = enemy->worldY;
  int dx[4] = {0, 1, 0, -1};
  int dy[4] = {1, 0, -1, 0};
  int i;
  int dist;
  
  for (i = 0; i < 4; i++) {
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
      if (dist < bestDist) {
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

  if (game->state == LOADING) {
    spawn_random_enemies(game, 4);
  }

  render(game);
  game->state = PLAYER_TURN;
  run_game(game);

  if (save_game(game, game->level->levelNumber, "./saves/save1"))
  {
    fprintf(stderr, "Error: Something went wrong while saving the game\n");
    return 1;
  }
  printf("Game has been saved!\n");
  cleanup_game(game);

  return 0;
}
