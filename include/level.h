struct Sprite;
struct Entity;

typedef struct LevelState
{
  unsigned int flags;
} LevelState;

typedef struct Level
{
  struct Sprite ***background;
  /* All map elements go in foreground; cast to the proper type after checking Entity->type */
  struct Entity ***foreground;
  LevelState levelState;
  int levelNumber;
  int enemyCount;
} Level;

typedef void *(*ENTITY_FACTORY)(int);
typedef void ***LEVEL_LAYER;

LevelState create_empty_level_state();
void cleanup_background(Level *level);
void cleanup_foreground(Level *level);
void cleanup_level(Level *level);
Level *create_empty_level(int levelNumber);
struct Sprite *sprite_from_number(int tileNo);
struct Entity *entity_from_number(int tileNo);
int load_layer(void ***layer, char *levelPrefix, char *levelSuffix, void *(*num_to_entity_func)(int));
Level *load_level(int levelNumber, int loadingFromSave);