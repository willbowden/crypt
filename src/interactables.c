#include "main.h"

void complete_level_1(Game *g)
{
  if ((g->level->levelState.flags & 1) == 0)
  {
    show_popup(g, "Careful adventurer! You are not prepared to enter. Arm  yourself first!");
  } else {
    g->level = load_level(2);
    set_player_pos(g, 13, WORLD_HEIGHT_SPRITES-1);
  }
}

void complete_level_2(Game *g, 

void progress_level(Game *g, int x, int y)
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
    case 2:

    default:
      break;
  }
}

void pickup_weapon(Game *g, int x, int y) 
{
  g->player->attack += 5;
  g->level->levelState.flags |= 1;
  free_interactable((Interactable *) g->level->foreground[y][x]);
  g->level->foreground[y][x] = NULL;
}

void pickup_armour(Game *g, int x, int y) 
{
  g->player->defense += 5;
  g->level->levelState.flags |= 1 << 1;
  free_interactable((Interactable *) g->level->foreground[y][x]);
  g->level->foreground[y][x] = NULL;
}