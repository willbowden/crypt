#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char **argv) {
    return 0;
}

Player * create_player(Sprite *sprite, int health, bool turn) {
    Player player = malloc(sizeof(Player));
    player->sprite = sprite;
    player->health = health;
    player->turn = turn;
}

Enemy * create_enemy(Sprite *sprite, int health, bool turn) {
    Enemy enemy = malloc(sizeof(Enemy));
    enemy->health = health;
    enemy->sprite = sprite;
    enemy->turn = turn;
}

Interactable * create_interactable(Sprite *sprite, bool passable) {
    Interactable interactable = malloc(sizeof(Interactable));
    interactable->passable = passable;
    return interactable;
}

void free_player(Player *player) {
    free(player->sprite);
    free(player);
}

void free_enemy(Enemy *enemy) {
    free(enemy->sprite);
    free(enemy);
}

void free_interactable(Interactable *interactable) {
    free(interactable->sprite);
    free(interactable);
}