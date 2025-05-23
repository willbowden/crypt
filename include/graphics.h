#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
#define SPRITESHEET_WIDTH_SPRITES 49
#define SPRITESHEET_HEIGHT_SPRITES 22
#define WORLD_HEIGHT_SPRITES 18
#define WORLD_WIDTH_SPRITES 32
#define HUD_HEIGHT_SPRITES 3
#define SCREEN_HEIGHT_SPRITES 21
#define SCREEN_WIDTH_SPRITES 32
#define RENDER_SCALE 3.0
#define MAX_ANIMATION_COUNT 16

struct Game;      /* Forward declaration – Game is defined in main.h */
struct Sprite;    /* Forward declaration */
struct GraphicsEngine;

typedef struct Animation
{
  int *targetX;
  int *targetY;
  struct Sprite *targetSprite;
  int currentFrame;
  int duration;
  int slot;
  void (*play)(struct GraphicsEngine *ge, struct Animation *a);
} Animation;

typedef struct GraphicsEngine
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *spritesheet;
  SDL_Texture *fontsheet;
  Animation **activeAnimations;
  unsigned short int animationSlots;
} GraphicsEngine;

typedef struct Sprite
{
  int tileNo;
  int spriteX;
  int spriteY;
  double angle;
  int flip;
} Sprite;

GraphicsEngine *initialise_graphics();
void cleanup_graphics(GraphicsEngine *ge);
void add_animation(GraphicsEngine *ge, int *targetX, int *targetY, Sprite *targetSprite, int duration, void (*play)(GraphicsEngine *ge, Animation *a));
void flashing_red_animation(GraphicsEngine *ge, Animation *a);
void draw_sprite(GraphicsEngine *ge, Sprite *sprite, int worldX, int worldY);
void draw_ascii_char(GraphicsEngine *ge, char c, int worldX, int worldY);
void render(struct Game *g);

#endif /* GRAPHICS_H */
