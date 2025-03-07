typedef enum {
    ENEMY, 
    PLAYER, 
    INTERACTABLE,
    FOREGROUND_TILE
} EntityType;

typedef enum {
    OGRE,
    BRUH
} EnemyType;

typedef struct Player {
    EntityType type;
    Sprite *sprite;
    int worldX;
    int worldY;
    int health;
} Player;

typedef struct Interactable {
    EntityType type;
    Sprite *sprite;
} Interactable;

typedef struct Enemy {
    EntityType type;
    Sprite *sprite;
    int health;
} Enemy;

typedef struct ForegroundTile {
    EntityType type;
    Sprite *sprite;
    int passable;
} ForegroundTile;

typedef struct Entity {
    EntityType type;
} Entity;

Enemy * create_enemy(Sprite *sprite, int health);
Interactable * create_interactable(Sprite *sprite, int passable);
ForegroundTile *create_foreground_tile(Sprite *sprite, int passable);

void free_enemy(Enemy *enemy);
void free_interactable(Interactable *interactable);
void free_foreground_tile(ForegroundTile *tile);
