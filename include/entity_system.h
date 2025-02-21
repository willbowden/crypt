#include <stdbool.h>

enum EntityType {
    ENEMY, 
    PLAYER, 
    INTERACTABLE    
}

enum EnemyType {
    OGRE,
    BRUH
}

typedef struct {
    Sprite *sprite
    int worldX;
    int worldY;
    int health;
    bool turn;
} Player;

typedef struct {
    Sprite *sprite
    bool passable;
} Interactable;

typedef struct {
    Sprite *sprite
    bool turn; /* boolean to decide whether its the enemies' turn */
    int health;
} Enemy;

Player * create_player(Sprite *sprite, int health, bool turn);
Enemy * create_enemy(Sprite *sprite, int health, bool turn);
Interactable * create_interactable(Sprite *sprite, bool passable);

void free_player(Player *player);
void free_enemy(Enemy *enemy);
void free_interactable(Interactable *interactable);