#include "SDL2/SDL_events.h"

struct Game;
struct Player;
struct Sprite;

#define BASE_PLAYER_ATTACK 5
#define BASE_PLAYER_DEFENSE 5

Player *create_player(struct Sprite *sprite, int health, int worldX, int worldY);
void free_player(Player *player);
void set_player_pos(struct Game *game, int newX, int newY);
void move_player(struct Game *game, SDL_KeyCode key);
void add_player(struct Game *game, int x, int y);
