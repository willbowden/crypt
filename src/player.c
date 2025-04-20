#include "main.h"

Player *create_player(Sprite *sprite, int health, int worldX, int worldY)
{
  Player *player = (Player *)malloc(sizeof(Player));
  player->sprite = sprite;
  player->health = health;
  player->attack = BASE_PLAYER_ATTACK;
  player->defense = BASE_PLAYER_DEFENSE;
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

void set_player_pos(Game *game, int newX, int newY)
{
  int oldX = game->player->worldX;
  int oldY = game->player->worldY;
  game->level->foreground[oldY][oldX] = NULL;
  game->level->foreground[newY][newX] = (Entity *)game->player;
  game->player->worldX = newX;
  game->player->worldY = newY;
}

void move_player(Game *game, SDL_Keycode key)
{
  int oldX = game->player->worldX;
  int oldY = game->player->worldY;
  int newX = oldX;
  int newY = oldY;
  Entity *target;

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
      newX < WORLD_WIDTH_SPRITES &&
      newX >= 0 &&
      newY < WORLD_HEIGHT_SPRITES &&
      newY >= 0)
  {
    target = game->level->foreground[newY][newX];

    if (target == NULL)
    {
      set_player_pos(game, newX, newY);
      game->state = ENEMY_TURN;
    }
    /* Process player walking onto an interactable */
    else if (game->level->foreground[newY][newX]->type == INTERACTABLE)
    {
      Interactable *interactable = (Interactable *)game->level->foreground[newY][newX];
      interactable->interact(game, newX, newY);
      if (interactable->passable) {
        set_player_pos(game, newX, newY);
        game->state = ENEMY_TURN;
      }
    }
    else if (target->type == ENEMY)
    {
      Enemy *enemy = (Enemy *)target;
      enemy->health -= game->player->attack;

      /* Add the red flashing animation to the enemy */
      add_animation(
            game->graphics,
            &enemy->worldX,
            &enemy->worldY,
            enemy->sprite,
            GAME_FPS / 4,
            &flashing_red_animation);
      
      /* Mark the enemy as having been attacked */
      enemy->wasAttacked = 1;
      
      if (enemy->health <= 0)
      {
        game->level->enemyCount--;
        free_enemy(enemy);
        game->level->foreground[newY][newX] = NULL;
      }
    }
  }
}

void add_player(Game *game)
{
  int x, y;

  if (game->player == NULL)
  {
    fprintf(stderr, "Player doesn't exist!\n");
    return;
  }

  x = game->player->worldX;
  y = game->player->worldY;

  if (game->level->foreground[y][x] != NULL)
  {
    free_player(game->player);
    fprintf(stderr, "Cannot place player on occupied tile!\n");
    return;
  }
  
  game->level->foreground[y][x] = (Entity *)game->player;
}

