#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

struct Game;

typedef enum {
    INVALID,
    ENEMY, 
    PLAYER, 
    INTERACTABLE,
    FOREGROUND_TILE
} EntityType;

typedef struct Player {
    EntityType type;
    Sprite *sprite;
    int worldX;
    int worldY;
    int health;
    int attack;
    int defense;
} Player;

typedef struct Interactable {
    EntityType type;
    Sprite *sprite;
    INTERACTABLES interactableType;
    InteractFunctionId funcId;
    void (*interact)(struct Game *g, int x, int y);
} Interactable;

typedef struct Enemy {
    /* Note: We now include persistent coordinates */
    EntityType entity_type;
    EnemyType enemy_type;
    Sprite *sprite;
    int health;
    int attack;
    int defense;
    SpecialAbility ability;
    int worldX;
    int worldY;
    int hasMoved;  /* New flag to ensure one move per turn */
} Enemy;

typedef struct ForegroundTile {
    EntityType type;
    Sprite *sprite;
    int passable;
} ForegroundTile;

typedef struct Entity {
    EntityType type;
} Entity;

Player * create_player(Sprite *sprite, int health, int worldX, int worldY);
Enemy * create_enemy(Sprite *sprite);
Interactable * create_interactable(Sprite *sprite, InteractFunctionId funcId, INTERACTABLES type);
ForegroundTile *create_foreground_tile(Sprite *sprite, int passable);

void free_enemy(Enemy *enemy);
void free_interactable(Interactable *interactable);
void free_foreground_tile(ForegroundTile *tile);

#endif /* ENTITY_SYSTEM_H */
