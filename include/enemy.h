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

typedef enum {
    ABILITY_NONE,
    ABILITY_TANK,
    ABILITY_INVISIBILITY,
    ABILITY_TELEPORTATION,
    ABILITY_SHARPNESS,
    ABILITY_POISON,
    ABILITY_FIREBALL
} AbilityType;

/* Function pointer for executing the special ability */
typedef void (*AbilityFunction) (struct Enemy *enemy, struct Game *game);

typedef struct {
    AbilityType type;
    AbilityFunction execute;
    int cooldown; /* -1 implies no cooldown */
} SpecialAbility;

struct Enemy *add_enemy(struct Game *game, int tileNo);
void spawn_random_enemies(struct Game *game, int count);
void enemy_turn(struct Game *game);
void template_execution(struct Enemy *enemy, struct Game *game);

#endif /* ENEMY_H */
