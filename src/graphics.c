#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "main.h"

GraphicsEngine *initialise_graphics()
{
  GraphicsEngine *graphics = (GraphicsEngine *)calloc(1, sizeof(GraphicsEngine));
  int screenWidth, screenHeight;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
    return NULL;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
  {
    fprintf(stderr, "SDL_image init failed: %s\n", IMG_GetError());
    return NULL;
  }

  screenWidth = SPRITE_WIDTH * WINDOW_WIDTH_SPRITES * RENDER_SCALE;
  screenHeight = SPRITE_HEIGHT * WINDOW_HEIGHT_SPRITES * RENDER_SCALE;

  graphics->window = SDL_CreateWindow("Crypt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  if (!graphics->window)
  {
    fprintf(stderr, "Window init failed: %s\n", SDL_GetError());
    return NULL;
  }

  graphics->renderer = SDL_CreateRenderer(graphics->window, -1, SDL_RENDERER_ACCELERATED);
  if (!graphics->renderer)
  {
    fprintf(stderr, "Renderer init failed: %s\n", SDL_GetError());
    return NULL;
  }

  graphics->spritesheet = IMG_LoadTexture(graphics->renderer, "./assets/spritesheet.png");
  if (!graphics->spritesheet)
  {
    fprintf(stderr, "Spritesheet init failed: %s\n", IMG_GetError());
    return NULL;
  }

  if (SDL_RenderSetScale(graphics->renderer, RENDER_SCALE, RENDER_SCALE) < 0)
  {
    fprintf(stderr, "Setting render scale failed: %s\n", IMG_GetError());
    return NULL;
  }

  return graphics;
}

void cleanup_graphics(GraphicsEngine *ge)
{
  if (!ge)
    return;

  if (ge->spritesheet)
  {
    SDL_DestroyTexture(ge->spritesheet);
  }
  if (ge->renderer)
  {
    SDL_DestroyRenderer(ge->renderer);
  }
  if (ge->window)
  {
    SDL_DestroyWindow(ge->window);
  }

  IMG_Quit();
  SDL_Quit();

  free(ge);
}

void clear_screen(GraphicsEngine *ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0, 0, 0, 255);
  SDL_RenderClear(ge->renderer);
}

void draw_sprite(GraphicsEngine *ge, Sprite *sprite, int worldX, int worldY)
{
  SDL_Rect spriteClip;
  SDL_Rect destRect;

  spriteClip.x = sprite->spriteX * SPRITE_WIDTH;
  spriteClip.y = sprite->spriteY * SPRITE_HEIGHT;
  spriteClip.w = SPRITE_WIDTH;
  spriteClip.h = SPRITE_HEIGHT;

  destRect.x = worldX * SPRITE_WIDTH;
  destRect.y = worldY * SPRITE_HEIGHT;
  destRect.w = SPRITE_WIDTH;
  destRect.h = SPRITE_HEIGHT;

  SDL_RenderCopyEx(ge->renderer, ge->spritesheet, &spriteClip, &destRect, sprite->angle, NULL, sprite->flip);
}

Sprite *sprite_from_entity(Entity *e)
{
  switch (e->type)
  {
  case ENEMY:
    return ((Enemy *)e)->sprite;
  case PLAYER:
    return ((Player *)e)->sprite;
  case INTERACTABLE:
    return ((Interactable *)e)->sprite;
  case FOREGROUND_TILE:
    return ((ForegroundTile *)e)->sprite;
  default:
    return NULL;
  }
}

void draw_level(GraphicsEngine *ge, Level *level)
{
  int x, y;

  for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
  {
    for (x = 0; x < WINDOW_WIDTH_SPRITES; x++)
    {
      if (level->foreground[y][x])
      {
        draw_sprite(ge, sprite_from_entity(level->foreground[y][x]), x, y);
      }
      else if (level->background[y][x])
      {
        draw_sprite(ge, level->background[y][x], x, y);
      }
    }
  }
}

void draw_screen(GraphicsEngine *ge, Level *level)
{
  draw_level(ge, level);
}

void present_frame(GraphicsEngine *ge)
{
  SDL_RenderPresent(ge->renderer);
}