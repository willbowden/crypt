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
    
    /* Phase 1: Enemies that start adjacent attack immediately */
    for (i = 0; i < WORLD_HEIGHT_SPRITES; i++) {
        for (j = 0; j < WORLD_WIDTH_SPRITES; j++) {
            Entity *ent = game->level->foreground[i][j];
            if (ent && ent->type == ENEMY) {
                Enemy *enemy = (Enemy *)ent;
                /* Check if enemy is adjacent (cardinal only) at the beginning of the turn */
                if (abs(enemy->worldX - game->player->worldX) + abs(enemy->worldY - game->player->worldY) == 1) {
                    game->player->health -= enemy->attack;
                    if (game->player->health <= 0)
                    {
                        setup_game_over_menu(game->menu);
                        game->state = MENU_OPEN;
                        return;
                    }
                    printf("Enemy attacked! Player HP: %d\n", game->player->health);
                    enemy->hasMoved = 1;  /* Mark as having acted */
                }
            }
        }
    }
    
    /* Phase 2: For enemies that did not attack at the start, move one tile toward the player */
    for (i = 0; i < WORLD_HEIGHT_SPRITES; i++) {
        for (j = 0; j < WORLD_WIDTH_SPRITES; j++) {
            Entity *ent = game->level->foreground[i][j];
            if (ent && ent->type == ENEMY) {
                Enemy *enemy = (Enemy *)ent;
                if (enemy->hasMoved)
                    continue;
                if (compute_next_move(game, enemy, &nextX, &nextY)) {
                    game->level->foreground[i][j] = NULL;
                    enemy->worldX = nextX;
                    enemy->worldY = nextY;
                    game->level->foreground[nextY][nextX] = (Entity *)enemy;
                }
                enemy->hasMoved = 1;
            }
        }
    }
    
    /* Reset the hasMoved flag for all enemies for the next enemy turn */
    for (i = 0; i < WORLD_HEIGHT_SPRITES; i++) {
        for (j = 0; j < WORLD_WIDTH_SPRITES; j++) {
            Entity *ent = game->level->foreground[i][j];
            if (ent && ent->type == ENEMY) {
                ((Enemy *)ent)->hasMoved = 0;
            }
        }
    }

    game->state = PLAYER_TURN;
}

void template_execution(struct Enemy *enemy, struct Game *game) {
    /* TODO: Not implemented yet, will implement after shortlisting the abilities and enemy types */
    /* Function to execute the special ability in game */
}