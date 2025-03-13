struct Sprite;
struct Entity;

typedef struct Level
{
  struct Sprite ***background;
  /**
   * Use generic tiles for all other map elements,
   * and cast to the correct pointer type after checking genericTile->type
   */
  struct Entity ***foreground;
} Level;

typedef void *(*ENTITY_FACTORY)(int);
typedef void ***LEVEL_LAYER;

void cleanup_background(Level *level);
void cleanup_foreground(Level *level);
void cleanup_level(Level *level);
Level *create_empty_level();
struct Sprite *sprite_from_number(int tileNo);
struct Entity *entity_from_number(int tileNo);
int load_layer(void ***layer, char *levelPrefix, char *levelSuffix, void *(*num_to_entity_func)(int));
Level *load_level(char *levelName);
