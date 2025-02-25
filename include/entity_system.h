#include <stdbool.h>

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

typedef struct {
    Sprite *sprite;
    int worldX;
    int worldY;
    int health;
    int turn;
} Player;

typedef struct {
    Sprite *sprite;
} Interactable;

typedef struct {
    Sprite *sprite;
    int turn; /* intean to decide whether its the enemies' turn */
    int health;
} Enemy;

typedef struct {
    Sprite *sprite;
    int passable;
} ForegroundTile;

typedef struct {
    EntityType type;
} GenericTile;

Player * create_player(Sprite *sprite, int health, int turn);
Enemy * create_enemy(Sprite *sprite, int health, int turn);
Interactable * create_interactable(Sprite *sprite, int passable);

void free_player(Player *player);
void free_enemy(Enemy *enemy);
void free_interactable(Interactable *interactable);
