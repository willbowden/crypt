#include "main.h"

struct Enemy *add_enemy(struct Game *game, int tileNo) {
    Enemy *enemy = create_enemy(sprite_from_number(tileNo));
    if(!enemy) {
        fprintf(stderr, "Failed to create enemy\n");
        return NULL;
    }

    switch (tileNo)
    {
    case MAGE:
        enemy->enemy_type = MAGE;
        enemy->health = 30;
        enemy->attack = 30;
        break;
    case SPIDER:
        enemy->enemy_type = SPIDER;
        enemy->health = 15;
        enemy->attack = 10; /* Implement poison damage for 2 turns */
        break;
    case GHOST:
        enemy->enemy_type = GHOST;
        enemy->health = 15;
        enemy->attack = 15;
        break;
    case GOBLIN:
        enemy->enemy_type = GOBLIN;
        enemy->health = 10;
        enemy->attack = 20;
        break;
    case GOON:
        enemy->enemy_type = GOON;
        enemy->health = 10;
        enemy->attack = 10;
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
    int i, j, nextX, nextY, raw, dmg;

    /* Phase 1: Enemies adjacent at the start of the turn attack,
       but only if they haven’t just been hit this turn */
    for (i = 0; i < WORLD_HEIGHT_SPRITES; i++) {
        for (j = 0; j < WORLD_WIDTH_SPRITES; j++) {
            Entity *ent = game->level->foreground[i][j];
            if (ent && ent->type == ENEMY) {
                Enemy *enemy = (Enemy *)ent;
                if (abs(enemy->worldX - game->player->worldX)
                  + abs(enemy->worldY - game->player->worldY) == 1) {
                    if (!enemy->wasAttacked) {
                        raw = enemy->attack;
                        dmg = raw - game->player->defense;
                        if (dmg < 1) dmg = 1;
                        game->player->health -= dmg;
                        if (game->player->health <= 0) {
                            setup_game_over_menu(game->menu);
                            game->state = MENU_OPEN;
                            return;
                        }
                    }
                    enemy->wasAttacked = 0;
                    enemy->hasMoved    = 1;
                }
            }
        }
    }
    /* Phase 2: Move any enemies that didn’t attack this turn */
    for (i = 0; i < WORLD_HEIGHT_SPRITES; i++) {
        for (j = 0; j < WORLD_WIDTH_SPRITES; j++) {
            Entity *ent = game->level->foreground[i][j];
            if (ent && ent->type == ENEMY) {
                Enemy *enemy = (Enemy *)ent;
                if (enemy->hasMoved) continue;
                if (compute_next_move(game, enemy, &nextX, &nextY)) {
                    game->level->foreground[i][j] = NULL;
                    enemy->worldX = nextX;
                    enemy->worldY = nextY;
                    game->level->foreground[nextY][nextX] = ent;
                }
                enemy->hasMoved = 1;
            }
        }
    }
    /* Reset move‐flags for the next cycle */
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