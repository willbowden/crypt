#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "main.h"

GraphicsEngine *initialise_graphics()
{
  GraphicsEngine *graphics = (GraphicsEngine *)malloc(sizeof(GraphicsEngine));
  int screenWidth, screenHeight;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL init failed: %s\n", SDL_GetError());
    return NULL;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
  {
    printf("SDL_image init failed: %s\n", IMG_GetError());
    SDL_Quit();
    return NULL;
  }

  screenWidth = SPRITE_WIDTH * WINDOW_WIDTH_SPRITES * RENDER_SCALE;
  screenHeight = SPRITE_HEIGHT * WINDOW_HEIGHT_SPRITES * RENDER_SCALE;

  graphics->window = SDL_CreateWindow("Crypt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  if (!graphics->window)
  {
    printf("Window init failed: %s\n", SDL_GetError());
    IMG_Quit();
    SDL_Quit();
    return NULL;
  }

  graphics->renderer = SDL_CreateRenderer(graphics->window, -1, SDL_RENDERER_ACCELERATED);
  if (!graphics->renderer)
  {
    printf("Renderer init failed: %s\n", SDL_GetError());
    SDL_DestroyWindow(graphics->window);
    IMG_Quit();
    SDL_Quit();
    return NULL;
  }

  graphics->spritesheet = IMG_LoadTexture(graphics->renderer, "./assets/spritesheet.png");
  if (!graphics->spritesheet)
  {
    printf("Spritesheet init failed: %s\n", IMG_GetError());
    SDL_DestroyRenderer(graphics->renderer);
    SDL_DestroyWindow(graphics->window);
    IMG_Quit();
    SDL_Quit();
    return NULL;
  }

  if (SDL_RenderSetScale(graphics->renderer, RENDER_SCALE, RENDER_SCALE) < 0) {
    printf("Setting render scale failed: %s\n", IMG_GetError());
    SDL_DestroyRenderer(graphics->renderer);
    SDL_DestroyWindow(graphics->window);
    IMG_Quit();
    SDL_Quit();
    return NULL;
  }

  graphics->render_frame = &render_frame;

  return graphics;
}

void cleanup_graphics(GraphicsEngine *ge)
{
  SDL_DestroyTexture(ge->spritesheet);
  SDL_DestroyRenderer(ge->renderer);
  SDL_DestroyWindow(ge->window);
  IMG_Quit();
  SDL_Quit();

  free(ge);
}

void render_sprite(GraphicsEngine *ge, Sprite *sprite, int worldX, int worldY)
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

  SDL_RenderCopy(ge->renderer, ge->spritesheet, &spriteClip, &destRect);
}

void render_frame(GraphicsEngine *ge)
{
  Sprite sprite = {24, 0};

  /* Clear screen */
  SDL_SetRenderDrawColor(ge->renderer, 0, 0, 0, 255);
  SDL_RenderClear(ge->renderer);

  /* Draw frame */
  
  render_sprite(ge, &sprite, 10, 10);

  /* Display frame */
  SDL_RenderPresent(ge->renderer);
}
