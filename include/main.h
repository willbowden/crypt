#include "level.h"
#include "graphics.h"
#include "enemy.h"
#include "entity_system.h"
#include "player.h"
#include "ui.h"
#include "interactables.h"
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
  int playerX;
  int playerY;
  int playerHealth;
  int levelNumber;
  LevelState levelState;
  Entity foregroundGrid[WORLD_HEIGHT_SPRITES][WORLD_WIDTH_SPRITES];
} SaveData;

void cleanup_game(Game *game);
int save_game(Game *game, int levelNumber, const char *saveFilename);
Game *load_game(const char *saveFilename);
void handle_keypress(Game *game, SDL_Event *e);
int initialise_game(Game *game, int levelNumber, Player *player);