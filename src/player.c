#include "main.h"

Player *create_player(Sprite *sprite, int health, int worldX, int worldY)
{
  Player *player = (Player *)malloc(sizeof(Player));
  player->sprite = sprite;
  player->health = health;
  player->worldX = worldX;
  player->worldY = worldY;
  player->type = PLAYER;
  return player;
}

void free_player(Player *player)
{
  free(player->sprite);
  free(player);
}

void move_player(Game *game, SDL_KeyCode key)
{
  int oldX = game->player->worldX;
  int oldY = game->player->worldY;
  int newX = oldX;
  int newY = oldY;

  switch (key)
  {
  case SDLK_UP:
    newY = oldY - 1;
    break;
  case SDLK_DOWN:
    newY = oldY + 1;
    break;
  case SDLK_LEFT:
    newX = oldX - 1;
    break;
  case SDLK_RIGHT:
    newX = oldX + 1;
    break;
  default:
    return;
  }

  if (
      newX < WINDOW_WIDTH_SPRITES &&
      newX >= 0 &&
      newY < WINDOW_HEIGHT_SPRITES &&
      newY >= 0)
  {
    if (game->level->foreground[newY][newX] == NULL)
    {
      game->level->foreground[oldY][oldX] = NULL;
      game->level->foreground[newY][newX] = (Entity *) game->player;
      game->player->worldX = newX;
      game->player->worldY = newY;
    }
  }
}

void add_player(Game *game, int x, int y)
{ 
  if(game->player == NULL)
  {
    fprintf(stderr, "%s\n", "Player doesn't exist!");
    return;
  }
  
  if (game->level->foreground[y][x] != NULL)
  {
    free_player(game->player);
    fprintf(stderr, "%s\n", "Cannot place player on occupied tile!");
    return;
  }

  game->level->foreground[y][x] = (Entity *)game->player;
}