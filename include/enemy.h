#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"

/* Based on spritesheet tile number */
typedef enum {
    MAGE = 74,
    GOBLIN = 128,
    SPIDER = 275,
    GHOST = 321,
    GOON = 124
} EnemyType;

struct Enemy;  /* Forward declaration; full definition comes from entity_system.h */
struct Game;

struct Enemy *add_enemy(struct Game *game, int tileNo);
void spawn_random_enemies(struct Game *game, EnemyType type, int count);
void enemy_turn(struct Game *game);

#endif /* ENEMY_H */
