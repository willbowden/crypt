#include "level.h"
#include "graphics.h"
#include "enemy.h"
#include "entity_system.h"
#include "player.h"
#include "ui.h"

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
} Game;

void cleanup_game(Game *game);
void handle_keypress(Game *game, SDL_Event *e);
int initialise_game(Game *game);
