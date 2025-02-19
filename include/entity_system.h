#include <graphics.h>

typedef struct {
    int worldX;
    int worldY;
    Sprite *sprite;
} Entity;

typedef struct {
    Entity base;
    int health;
    bool turn;
} Player;

typedef struct {
    Entity base;
    bool passable;
} Interactable;

typedef struct {
    Entity base;
    bool turn; /* boolean to decide whether its the enemies' turn */
    int health;
} Enemy;

void create_entity(Entity *entity, int worldX, int worldY, Sprite *sprite);
void create_player(Player *player, Entity base, int health, bool turn);
void create_enemy(Enemy *enemy, Entity base, int health, bool turn);
void create_interactable(Interactable *interactable, bool passable);

void free_entity(Entity *entity);
void free_player(Player *player);
void free_enemy(Enemy *enemy);
void free_interactable(Interactable *interactable);