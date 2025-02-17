#include "level.h"
#include "graphics.h"

typedef struct Game
{
  GraphicsEngine *graphics;
  Level *level;
} Game;

void cleanup_game(Game *game);
