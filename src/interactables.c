#include "main.h"

void complete_level_1(Game *g)
{
  return;
}

void progress_level(Game *g)
{
  /**
   * Depending on the current level, progression
   *  will have different preconditions
   */
  switch (g->level->levelNumber)
  {
    case 1:
      complete_level_1(g);
      break;
    default:
      break;
  }
}