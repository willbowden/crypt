struct Sprite;

/** TODO HERE
 * When Player/Entity structs are defined, e.g:
 * struct Entity {
 *   int worldX;
 *   int worldY;
 *   Sprite sprite;
 *   int passable;
 * }
 * I can then add the relevant layers to the level
*/
typedef struct Level
{
  struct Sprite ***background;
} Level;

void cleanup_level(Level *level);
Level *create_empty_level();
int load_background(Level *level, char *levelName);
Level *load_level(char *levelName);
