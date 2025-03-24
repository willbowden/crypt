#include "level.h"
#include "graphics.h"
#include "enemy.h"
#include "entity_system.h"
#include "player.h"
#include "string.h"
#include <stdio.h>

typedef struct Game
{
  GraphicsEngine *graphics;
  Level *level;
  Player *player;
} Game;

typedef struct {
  int playerX;
  int playerY;
  int playerHealth;
  char levelName[64];
  Entity foregroundGrid[WINDOW_HEIGHT_SPRITES][WINDOW_WIDTH_SPRITES];
} SaveData;

void cleanup_game(Game *game);
int save_game(Game *game, const char *levelName, const char *saveFilename);
Game *load_game(const char *saveFilename);
void handle_keypress(Game *game, SDL_Event *e);
int initialise_game(Game *game, char *levelName, Player *player);
