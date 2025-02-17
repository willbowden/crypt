#include <SDL2/SDL.h>

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
#define WINDOW_HEIGHT_SPRITES 18
#define WINDOW_WIDTH_SPRITES 32
#define RENDER_SCALE 3.0

typedef struct GraphicsEngine
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *spritesheet;
  void (*render_frame)(struct GraphicsEngine *);
} GraphicsEngine;

typedef struct
{
  int spriteX;
  int spriteY;

} Sprite;

GraphicsEngine *initialise_graphics();
void cleanup_graphics(GraphicsEngine *ge);
void render_frame(GraphicsEngine *ge);
