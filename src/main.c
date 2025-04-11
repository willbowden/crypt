#include "main.h"
#include "enemy.h"
#include "entity_system.h"

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
    int enemyCount;  /* Count of enemy cells */
    SaveData data;
    FILE *file;
    
    file = fopen(saveFilename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not create/open the save file\n");
        return 1;
    }
    
    data.playerX = game->player->worldX;
    data.playerY = game->player->worldY;
    data.playerHealth = game->player->health;
    strncpy(data.levelName, levelName, strlen(levelName));
    data.levelName[strlen(levelName)] = '\0';
    
    /* Copy foreground grid base data (only the base Entity part) */
    for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++) {
        for (x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
            if (game->level->foreground[y][x] == NULL) {
                data.foregroundGrid[y][x].type = INVALID;
            }
            else {
                data.foregroundGrid[y][x] = *(game->level->foreground[y][x]);
            }
        }
    }
    fwrite(&data, sizeof(SaveData), 1, file);
    
    /* Count enemies */
    enemyCount = 0;
    for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++) {
        for (x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
            if (game->level->foreground[y][x] != NULL &&
                game->level->foreground[y][x]->type == ENEMY) {
                enemyCount++;
            }
        }
    }
    fwrite(&enemyCount, sizeof(int), 1, file);
    
    /* Write enemy extra state */
    for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++) {
        for (x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
            if (game->level->foreground[y][x] != NULL &&
                game->level->foreground[y][x]->type == ENEMY) {
                Enemy *enemy = (Enemy *)game->level->foreground[y][x];
                /* Write grid position (x and y) */
                fwrite(&x, sizeof(int), 1, file);
                fwrite(&y, sizeof(int), 1, file);
                fwrite(&enemy->worldX, sizeof(int), 1, file);
                fwrite(&enemy->worldY, sizeof(int), 1, file);
                fwrite(&enemy->health, sizeof(int), 1, file);
                fwrite(&enemy->attack, sizeof(int), 1, file);
                fwrite(&enemy->defense, sizeof(int), 1, file);
                fwrite(&enemy->enemy_type, sizeof(EnemyType), 1, file);
            }
        }
    }
    
    fclose(file);
    return 0;
}

Game *load_game(const char *saveFilename)
{
    int x, y;
    int found;
    int enemyCount;
    int i;
    int tileX, tileY;
    int worldX, worldY, health, attack, defense;
    EnemyType enemyType;
    SaveData loadData;
    Player *player;
    FILE *file;
    Game *game;
    
    found = 1;
    file = fopen(saveFilename, "rb");
    game = (Game *)malloc(sizeof(Game));
    if (game == NULL) {
        fprintf(stderr, "Error: Could not initiaze game");
        return NULL;
    }
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open the save file. Starting a new game.\n");
        game->state = LOADING;
        found = 0;
        loadData.playerX = 29;
        loadData.playerY = 5;
        loadData.playerHealth = 100;
        strcpy(loadData.levelName, "./assets/Levels/Level1");
    }
    else {
        fread(&loadData, sizeof(SaveData), 1, file);
        game->state = PLAYER_TURN;
    }
    
    player = create_player(sprite_from_number(25), loadData.playerHealth, loadData.playerX, loadData.playerY);
    if (player == NULL) {
        fprintf(stderr, "Error: Could not create player");
        return NULL;
    }
    if (initialise_game(game, loadData.levelName, player)) {
        fprintf(stderr, "Error: Failed to initialize game.\n");
        return NULL;
    }
    add_player(game, loadData.playerX, loadData.playerY);
    
    if (found) {
        for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++) {
            for (x = 0; x < WINDOW_WIDTH_SPRITES; x++) {
                if (loadData.foregroundGrid[y][x].type == INVALID) {
                    game->level->foreground[y][x] = NULL;
                }
                else {
                    if (game->level->foreground[y][x] == NULL) {
                        game->level->foreground[y][x] = (Entity *)malloc(sizeof(Entity));
                    }
                    if (game->level->foreground[y][x] == NULL) {
                        fprintf(stderr, "Error: Could not allocate memory for foreground entity\n");
                        return NULL;
                    }
                    *(game->level->foreground[y][x]) = loadData.foregroundGrid[y][x];
                }
            }
        }
        
        /* Restore enemy extra state */
        fread(&enemyCount, sizeof(int), 1, file);
        for (i = 0; i < enemyCount; i++) {
            fread(&tileX, sizeof(int), 1, file);
            fread(&tileY, sizeof(int), 1, file);
            fread(&worldX, sizeof(int), 1, file);
            fread(&worldY, sizeof(int), 1, file);
            fread(&health, sizeof(int), 1, file);
            fread(&attack, sizeof(int), 1, file);
            fread(&defense, sizeof(int), 1, file);
            fread(&enemyType, sizeof(EnemyType), 1, file);
            {
                Enemy *enemy = add_enemy(game, enemyType);
                if (enemy == NULL) {
                    fprintf(stderr, "Error: Could not create enemy from save data\n");
                    continue;
                }
                enemy->worldX = worldX;
                enemy->worldY = worldY;
                enemy->health = health;
                enemy->attack = attack;
                enemy->defense = defense;
                enemy->enemy_type = enemyType;
                game->level->foreground[tileY][tileX] = (Entity *)enemy;
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
  char *levelName = "./assets/Levels/Level1";
  
  if(game == NULL) {
    fprintf(stderr, "Error: Unable to initialize game");
    return 1;
  }

  if (game->state == LOADING) {
    spawn_random_enemies(game, 4);
  }

  render(game);
  game->state = PLAYER_TURN;
  run_game(game);

  if (save_game(game, levelName, "./saves/save1"))
  {
    fprintf(stderr, "Error: Something went wrong while saving the game\n");
    return 1;
  }
  printf("Game has been saved!\n");
  cleanup_game(game);

  return 0;
}
