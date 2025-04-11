#ifndef MAIN_H
#define MAIN_H

#include "level.h"
#include "graphics.h"
#include "interactables.h"
#include "enemy.h"
#include "entity_system.h"
#include "player.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include "limits.h"

#define GAME_FPS 60

typedef enum GameState {
  LOADING,
  PLAYER_TURN,
  ENEMY_TURN,
  PAUSED,
  MENU_OPEN,
  DIALOG_OPEN
} GameState;

typedef struct Game
{
  GraphicsEngine *graphics;
  Level *level;
  Player *player;
  GameState state;
  UI *ui;
} Game;

typedef struct {
  Player player;
  int levelNumber;
  LevelState levelState;
  int numEntities;
} SaveHeader;

typedef struct {
  int x;
  int y;
  EntityType type;
} ForegroundDataHeader;

/* Save data for the enemy */
typedef struct {
  int health;
  EnemyType enemyType;
} EnemyData;

/* Save data for interactable */
typedef struct {
  InteractFunctionId funcId;
  INTERACTABLES type;
} InteractableData;


void cleanup_game(Game *game);
int save_game(Game *game, int levelNumber, const char *saveFilename);
Game *load_game(const char *saveFilename);
void handle_keypress(Game *game, SDL_Event *e);
int initialise_game(Game *game, int levelNumber, Player *player);
int compute_next_move(Game *game, Enemy *enemy, int *nextX, int *nextY);

#endif /* MAIN_H */
