#include <stdio.h>
#include <stdlib.h>
#include <entity_system.h>

int main(int argc, char **argv) {
    return 0;
}

void create_entity(Entity *entity, int worldX, int worldY, Sprite *sprite) {
    entity = malloc(sizeof(Entity));
    entity->sprite = sprite;
    entity->worldX = worldX;
    entity->worldY = worldY;
}

void create_player(Player *player, Entity base, int health, bool turn) {
    player = malloc(sizeof(Player));
    player->base = base;
    player->health = health;
    player->turn = turn;
}

void create_enemy(Enemy *enemy, Entity base, int health, bool turn) {
    enemy = malloc(sizeof(Enemy));
    enemy->base = base;
    enemy->health = health;
    enemy->turn = turn;
}

void create_interactable(Interactable *interactable, Entity base, bool passable) {
    interactable = malloc(sizeof(Interactable));
    interactable->base = base;
    interactable->passable = passable;
}

void free_entity(Entity *entity) {
    free(entity->sprite);
    free(entity);
}

void free_player(Player *player) {
    free(player->base);
    free(player);
}

void free_enemy(Enemy *enemy) {
    free(enemy->base);
    free(enemy);
}

void free_interactable(Interactable *interactable) {
    free(interactable);
}