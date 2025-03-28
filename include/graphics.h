#include <SDL2/SDL.h>

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
#define SPRITESHEET_WIDTH_SPRITES 49
#define SPRITESHEET_HEIGHT_SPRITES 22
#define WINDOW_HEIGHT_SPRITES 18
#define WINDOW_WIDTH_SPRITES 32
#define RENDER_SCALE 3.0

typedef struct GraphicsEngine
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *spritesheet;
  SDL_Texture *fontsheet;
} GraphicsEngine;

typedef struct Sprite
{
  int spriteX;
  int spriteY;
  double angle;
  int flip;

} Sprite;

GraphicsEngine *initialise_graphics();
void cleanup_graphics(GraphicsEngine *ge);
void clear_screen(GraphicsEngine *ge);
void draw_sprite(GraphicsEngine *ge, Sprite *sprite, int worldX, int worldY);
void draw_level(GraphicsEngine *ge, Level *level);
void present_frame(GraphicsEngine *ge);