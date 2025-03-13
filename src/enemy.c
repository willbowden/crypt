#include "main.h"

struct Enemy *add_enemy(struct Game *game, int tileNo) {
    Enemy *enemy = create_enemy(sprite_from_number(tileNo));
    SpecialAbility ability = {ABILITY_NONE, NULL, -1};
    if(!enemy) {
        fprintf(stderr, "Failed to create enemy\n");
        return NULL;
    }

    switch (tileNo)
    {
    case MAGE:
        ability.type = ABILITY_FIREBALL;
        ability.execute = template_execution;
        ability.cooldown = 2;
        enemy->enemy_type = MAGE;
        enemy->health = 30;
        enemy->attack_damage = 30;
        enemy->ability = ability;
        break;
    case SPIDER:
        ability.type = ABILITY_POISON;
        ability.execute = template_execution;
        ability.cooldown = 2;
        enemy->enemy_type = SPIDER;
        enemy->health = 15;
        enemy->attack_damage = 10; /* Implement poison damage for 2 turns */
        enemy->ability = ability;
        break;
    case GHOST:
        ability.type = ABILITY_TELEPORTATION;
        ability.execute = template_execution;
        ability.cooldown = 3;
        enemy->enemy_type = GHOST;
        enemy->health = 15;
        enemy->attack_damage = 15;
        enemy->ability = ability;
        break;
    case GOBLIN:
        ability.type = ABILITY_SHARPNESS;
        ability.execute = template_execution;
        enemy->enemy_type = GOBLIN;
        enemy->health = 10;
        enemy->attack_damage = 20;
        enemy->ability = ability;
        break;
    case GOON:
        enemy->enemy_type = GOON;
        enemy->health = 10;
        enemy->attack_damage = 10;
        enemy->ability = ability;
        break;
    default:
        fprintf(stderr, "%s\n", "Incorrect tileNo provided for enemy creation");
        break;
    }

    return enemy;
}

void template_execution(struct Enemy *enemy, struct Game *game) {
    /* TODO: Not implemented yet, will implement after shortlisting the abilities and enemy types */
    /* Function to execute the special ability in game */
}