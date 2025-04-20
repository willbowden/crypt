struct Game;

typedef enum INTERACTABLES {
  LEVEL1_DOOR = 838,
  LEVEL1_SWORD = 376,
  LEVEL2_ARMOUR = 32,
  LEVEL3_SWORD = 381,
  TRANSITION_ARROW = 612,
  WINNING_CROWN = 142
} INTERACTABLES;

void progress_level(struct Game *g, int x, int y);
void complete_game(struct Game *g, int x, int y);
void pickup_weapon(struct Game *g, int x, int y);
void pickup_armour(struct Game *g, int x, int y);