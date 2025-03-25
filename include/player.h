#include "SDL2/SDL_events.h"

struct Game;
struct Player;
struct Sprite;


Player *create_player(struct Sprite *sprite, int health, int worldX, int worldY);
void free_player(Player *player);
void move_player(struct Game *game, SDL_KeyCode key);
struct Player *add_player(struct Game *game);