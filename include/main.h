#include "level.h"
#include "graphics.h"
#include "enemy.h"
#include "entity_system.h"
#include "player.h"

typedef struct Game
{
  GraphicsEngine *graphics;
  Level *level;
  Player *player;
} Game;

void cleanup_game(Game *game);
void handle_keypress(Game *game, SDL_Event *e);
int initialise_game(Game *game);
