#include "level.h"
#include "graphics.h"
#include "entity_system.h"

typedef struct Game
{
  GraphicsEngine *graphics;
  Level *level;
} Game;

void cleanup_game(Game *game);
