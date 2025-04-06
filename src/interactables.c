#include "main.h"

void complete_level_1(Game *g)
{
  if (g->level->levelState.flags == 0)
  {
    show_popup(g, "Careful adventurer! You are not prepared to enter. Arm yourself first!");
  } else {
    g->level = load_level(2);
    set_player_pos(g, 13, WINDOW_HEIGHT_SPRITES-1);
  }
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