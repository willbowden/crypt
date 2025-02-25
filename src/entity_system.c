#include <stdio.h>
#include <stdlib.h>
#include "main.h"

Player *create_player(Sprite *sprite, int health, int turn)
{
    Player *player = (Player *)malloc(sizeof(Player));
    player->sprite = sprite;
    player->health = health;
    player->turn = turn;
    return player;
}

Enemy *create_enemy(Sprite *sprite, int health, int turn)
{
    Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
    enemy->health = health;
    enemy->sprite = sprite;
    enemy->turn = turn;
    return enemy;
}

Interactable *create_interactable(Sprite *sprite, int passable)
{
    Interactable *interactable = (Interactable *)malloc(sizeof(Interactable));
    interactable->sprite = sprite;
    return interactable;
}

ForegroundTile *create_foreground_tile(Sprite *sprite, int passable)
{
    ForegroundTile *tile = (ForegroundTile *)malloc(sizeof(ForegroundTile));
    tile->sprite = sprite;
    tile->passable = passable;
    return tile;
}

void free_player(Player *player)
{
    free(player->sprite);
    free(player);
}

void free_enemy(Enemy *enemy)
{
    free(enemy->sprite);
    free(enemy);
}

void free_interactable(Interactable *interactable)
{
    free(interactable->sprite);
    free(interactable);
}

void free_foreground_tile(ForegroundTile *tile)
{
    free(tile->sprite);
    free(tile);
}