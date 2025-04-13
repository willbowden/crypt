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
#define PATH_SEPARATOR '/'
#define SAVE_INTERVAL 15

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
  int playerWorldX;
  int playerWorldY;
  int playerSpriteNo;
  int playerHealth;
  int playerAttack;
  int playerDefense;
  int levelNumber;
  LevelState levelState;
  int numEntities;
} SaveHeader;

typedef struct {
  int x;
  int y;
  int tileNo;
  EntityType entityType;
} ForegroundDataHeader;

void cleanup_game(Game *game);
Game *initialise_game();
Game *new_game();
int save_game(Game *game, const char *saveFilename);
Game *load_game(const char *saveFilename);
void handle_keypress(Game *game, SDL_Event *e);
int compute_next_move(Game *game, Enemy *enemy, int *nextX, int *nextY);

#endif /* MAIN_H */
