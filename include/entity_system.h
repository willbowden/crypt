#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#define MAX_DEFENSE 30
#define MAX_ATTACK 30
#define MAX_HEALTH 100

struct Game;

typedef enum {
    INVALID,
    ENEMY, 
    PLAYER, 
    INTERACTABLE,
    FOREGROUND_TILE
} EntityType;

typedef struct Interactable {
    EntityType type;
    Sprite *sprite;
    int passable;
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
    int worldX;
    int worldY;
    int hasMoved;  
    int wasAttacked;
} Enemy;

typedef struct ForegroundTile {
    EntityType type;
    Sprite *sprite;
    int passable;
} ForegroundTile;

typedef struct Entity {
    EntityType type;
    Sprite *sprite;
} Entity;

Enemy * create_enemy(Sprite *sprite);
Interactable * create_interactable(Sprite *sprite, int passable, void (*interact)(struct Game *g, int x, int y));
ForegroundTile *create_foreground_tile(Sprite *sprite, int passable);

void free_enemy(Enemy *enemy);
void free_interactable(Interactable *interactable);
void free_foreground_tile(ForegroundTile *tile);

#endif /* ENTITY_SYSTEM_H */
