#include "main.h"

void complete_level_1(Game *g)
{
  /**
   * Check if player has picked up a weapon
   */
  if ((g->level->levelState.flags & 1) == 0)
  {
    show_popup(g, "Only a fool would enter this place without a weapon!");
  }
  else if (g->level->enemyCount != 0)
  {
    show_popup(g, "Don't leave any foes behind!");
  }
  else
  {
    g->level = load_level(2, 0);
    set_player_pos(g, 13, WORLD_HEIGHT_SPRITES - 1);
  }
}

void complete_level_2(Game *g)
{
  if ((g->level->levelState.flags & (1 << 1)) == 0)
  {
    show_popup(g, "You can't go on with an outfit like that!");
  }
  else if (g->level->enemyCount != 0)
  {
    show_popup(g, "Don't leave any foes behind!");
  }
  else
  {
    g->level = load_level(3, 0);
    set_player_pos(g, WORLD_WIDTH_SPRITES / 2, WORLD_HEIGHT_SPRITES - 1);
    show_popup(g, "Goons!");
    spawn_random_enemies(g, GOON, 5);
  }
}

void complete_level_3(Game *g)
{
  if ((g->level->levelState.flags & 1) == 0)
  {
    show_popup(g, "You'll need a stronger weapon to survive in there!");
  }
  else if (g->level->enemyCount != 0)
  {
    show_popup(g, "Don't leave any foes behind!");
  }
  else
  {
    g->level = load_level(4, 0);
    set_player_pos(g, WORLD_WIDTH_SPRITES / 2, WORLD_HEIGHT_SPRITES - 1);
  }
}

void complete_level_4(Game *g)
{
  g->level = load_level(5, 0);
  set_player_pos(g, WORLD_WIDTH_SPRITES / 2, WORLD_HEIGHT_SPRITES - 1);
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
  
  Sprite *old_sprite;
  g->player->attack += 5;
  g->level->levelState.flags |= 1;
  free_interactable((Interactable *)g->level->foreground[y][x]);
  g->level->foreground[y][x] = NULL;

  switch (g->level->levelNumber)
  {
  case 1:
    old_sprite = g->player->sprite;
    g->player->sprite = sprite_from_number(26);
    free(old_sprite);
    break;
  }
}

void pickup_armour(Game *g, int x, int y)
{
  Sprite *old_sprite;

  g->player->defense += 5;
  g->level->levelState.flags |= 1 << 1;
  free_interactable((Interactable *)g->level->foreground[y][x]);
  g->level->foreground[y][x] = NULL;

  switch (g->level->levelNumber)
  {
  case 2:
    old_sprite = g->player->sprite;
    g->player->sprite = sprite_from_number(28);
    free(old_sprite);
    break;
  }
}