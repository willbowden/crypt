#include "graphics.h"

typedef struct
{
  GraphicsEngine *graphics;
} Game;

void cleanup_game(Game *game);
