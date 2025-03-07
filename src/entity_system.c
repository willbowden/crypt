#include <stdio.h>
#include <stdlib.h>
#include "main.h"

Enemy *create_enemy(Sprite *sprite, int health)
{
    Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
    enemy->health = health;
    enemy->sprite = sprite;
    enemy->type = ENEMY;
    return enemy;
}

Interactable *create_interactable(Sprite *sprite, int passable)
{
    Interactable *interactable = (Interactable *)malloc(sizeof(Interactable));
    interactable->sprite = sprite;
    interactable->type = INTERACTABLE;
    return interactable;
}

ForegroundTile *create_foreground_tile(Sprite *sprite, int passable)
{
    ForegroundTile *tile = (ForegroundTile *)malloc(sizeof(ForegroundTile));
    tile->sprite = sprite;
    tile->passable = passable;
    tile->type = FOREGROUND_TILE;
    return tile;
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