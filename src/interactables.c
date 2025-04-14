#include "main.h"

void complete_level_1(Game *g)
{
  if ((g->level->levelState.flags & 1) == 0)
  {
    show_popup(g, "Careful adventurer! You are not prepared to enter. Arm  yourself first!");
  } else {
    g->level = load_level(2, 0);
    set_player_pos(g, 13, WORLD_HEIGHT_SPRITES-1);
  }
}

void complete_level_2(Game *g)
{
  g->level = load_level(3, 0);
  set_player_pos(g, WORLD_WIDTH_SPRITES / 2, WORLD_HEIGHT_SPRITES-1);
}

void complete_level_3(Game *g)
{
  g->level = load_level(4, 0);
  set_player_pos(g, WORLD_WIDTH_SPRITES / 2, WORLD_HEIGHT_SPRITES-1);
}

void complete_level_4(Game *g)
{
  g->level = load_level(5, 0);
  set_player_pos(g, WORLD_WIDTH_SPRITES / 2, WORLD_HEIGHT_SPRITES-1);
}

/* void complete_level_5(Game *g)
// {
//   g->level = load_level(3, 0);
//   set_player_pos(g, WORLD_WIDTH_SPRITES / 2, WORLD_HEIGHT_SPRITES-1);
// } */

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
      complete_level_2(g);
      break;
    case 3:
      complete_level_3(g);
      break;
    case 4:
      complete_level_4(g);
      break;
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