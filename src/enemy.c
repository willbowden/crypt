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
        enemy->attack = 30;
        enemy->ability = ability;
        break;
    case SPIDER:
        ability.type = ABILITY_POISON;
        ability.execute = template_execution;
        ability.cooldown = 2;
        enemy->enemy_type = SPIDER;
        enemy->health = 15;
        enemy->attack = 10; /* Implement poison damage for 2 turns */
        enemy->ability = ability;
        break;
    case GHOST:
        ability.type = ABILITY_TELEPORTATION;
        ability.execute = template_execution;
        ability.cooldown = 3;
        enemy->enemy_type = GHOST;
        enemy->health = 15;
        enemy->attack = 15;
        enemy->ability = ability;
        break;
    case GOBLIN:
        ability.type = ABILITY_SHARPNESS;
        ability.execute = template_execution;
        enemy->enemy_type = GOBLIN;
        enemy->health = 10;
        enemy->attack = 20;
        enemy->ability = ability;
        break;
    case GOON:
        enemy->enemy_type = GOON;
        enemy->health = 10;
        enemy->attack = 10;
        enemy->ability = ability;
        break;
    default:
        fprintf(stderr, "%s\n", "Incorrect tileNo provided for enemy creation");
        return NULL;
        break;
    }

    return enemy;
}

void spawn_random_enemies(Game *game, EnemyType type, int count) {
    int freeTiles[WORLD_WIDTH_SPRITES * WORLD_HEIGHT_SPRITES][2];
    int freeCount = 0;
    int x, y, i;
    /* Scan the level for free tiles (where foreground is NULL) */
    for (y = 0; y < WORLD_HEIGHT_SPRITES; y++) {
        for (x = 0; x < WORLD_WIDTH_SPRITES; x++) {
            if (game->level->foreground[y][x] == NULL) {
                freeTiles[freeCount][0] = x;
                freeTiles[freeCount][1] = y;
                freeCount++;
            }
        }
    }
    /* Spawn 'count' enemies at random free positions */
    for (i = 0; i < count && freeCount > 0; i++) {
        int idx = rand() % freeCount;
        int spawnX = freeTiles[idx][0];
        int spawnY = freeTiles[idx][1];
        Enemy *enemy = add_enemy(game, type);  /* For example, spawn a MAGE */ 
        if (enemy != NULL) {
            enemy->worldX = spawnX;
            enemy->worldY = spawnY;
            game->level->foreground[spawnY][spawnX] = (Entity *)enemy;
        }
        /* Remove the used position from freeTiles (swap with last and decrement freeCount) */ 
        freeTiles[idx][0] = freeTiles[freeCount-1][0];
        freeTiles[idx][1] = freeTiles[freeCount-1][1];
        freeCount--;
    }

    game->level->enemyCount += count;
}

void enemy_turn(Game *game) {
    int i, j, nextX, nextY;
    /* Iterate over the grid to move each enemy */
    for (i = 0; i < WORLD_HEIGHT_SPRITES; i++) {
        for (j = 0; j < WORLD_WIDTH_SPRITES; j++) {
            Entity *ent = game->level->foreground[i][j];
            if (ent && ent->type == ENEMY) {
                Enemy *enemy = (Enemy *)ent;
                /* Skip enemy if it has already moved this turn */
                if (enemy->hasMoved)
                    continue;
                /* Compute next step (which, with our compute_next_move change,
                   moves only one tile at a time) */
                if (compute_next_move(game, enemy, &nextX, &nextY)) {
                    /* If the next tile is occupied by the player, initiate attack */
                    if (game->level->foreground[nextY][nextX] &&
                        game->level->foreground[nextY][nextX]->type == PLAYER) {
                        /* TODO: Attack logic here */
                    } else {
                        /* Move the enemy: update the grid and enemy coordinates */
                        game->level->foreground[i][j] = NULL;
                        enemy->worldX = nextX;
                        enemy->worldY = nextY;
                        game->level->foreground[nextY][nextX] = (Entity *)enemy;
                    }
                    /* Mark enemy as having moved this turn */
                    enemy->hasMoved = 1;
                }
            }
        }
    }
    
    /* After processing enemy moves, reset the hasMoved flag for all enemies */
    for (i = 0; i < WORLD_HEIGHT_SPRITES; i++) {
        for (j = 0; j < WORLD_WIDTH_SPRITES; j++) {
            Entity *ent = game->level->foreground[i][j];
            if (ent && ent->type == ENEMY) {
                ((Enemy *)ent)->hasMoved = 0;
            }
        }
    }
}

void template_execution(struct Enemy *enemy, struct Game *game) {
    /* TODO: Not implemented yet, will implement after shortlisting the abilities and enemy types */
    /* Function to execute the special ability in game */
}