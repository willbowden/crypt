struct Game;

typedef enum {
  INTERACT_NONE,
  INTERACT_PROGRESS_LEVEL,
  INTERACT_PICKUP_EQUIPMENT
} InteractFunctionId;

typedef enum INTERACTABLES {
  LEVEL1_DOOR = 838,
  LEVEL1_SWORD = 376
} INTERACTABLES;

void progress_level(struct Game *g, int x, int y);
void complete_level_1(struct Game *g, int x, int y);
void pickup_equipment(struct Game *g, int x, int y);