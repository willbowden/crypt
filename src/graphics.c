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

  graphics->fontsheet = IMG_LoadTexture(graphics->renderer, "./assets/fontsheet.png");
  if (!graphics->fontsheet)
  {
    fprintf(stderr, "Font sheet init failed: %s\n", IMG_GetError());
    return NULL;
  }

  if (SDL_RenderSetScale(graphics->renderer, RENDER_SCALE, RENDER_SCALE) < 0)
  {
    fprintf(stderr, "Setting render scale failed: %s\n", IMG_GetError());
    return NULL;
  }

  graphics->activeAnimations = (Animation **)calloc(MAX_ANIMATION_COUNT, sizeof(Animation *));
  if (!graphics->activeAnimations)
  {
    fprintf(stderr, "%s", "Error initialising active animations list");
    return NULL;
  }

  graphics->animationSlots = USHRT_MAX;

  return graphics;
}

void add_animation(GraphicsEngine *ge, int *targetX, int *targetY, Sprite *targetSprite, int duration, void (*play)(GraphicsEngine *ge, Animation *a))
{
  Animation *a = calloc(1, sizeof(Animation));
  int x;
  int slot = -1;
  if (a == NULL)
  {
    fprintf(stderr, "Error creating animation for x: %d y: %d", *targetX, *targetY);
    return;
  }

  a->targetX = targetX;
  a->targetY = targetY;
  a->targetSprite = targetSprite;
  a->currentFrame = 0;
  a->duration = duration;
  a->play = play;

  /**
   * Find empty animation slot
   */
  for (x = 0; x < MAX_ANIMATION_COUNT; x++)
  {
    /* Check binary flags for empty slots (empty if bit = 1) */
    if (((1 << x) & ge->animationSlots) > 0)
    {
      slot = x;
      /* Set newly occupied slot's bit to 0 */
      ge->animationSlots = ge->animationSlots ^ (1 << x);
      break;
    }
  }

  if (slot == -1)
  {
    fprintf(stderr, "%s", "Animation slots are full!");
    free(a);
    return;
  }

  a->slot = slot;
  ge->activeAnimations[slot] = a;
}

void flashing_red_animation(GraphicsEngine *ge, Animation *a)
{
  if (a->currentFrame % (a->duration / 4) < (a->duration / 8))
  {
    SDL_SetTextureColorMod(ge->spritesheet, 255, 0, 0);
    draw_sprite(ge,
                a->targetSprite,
                *a->targetX,
                *a->targetY);
    SDL_SetTextureColorMod(ge->spritesheet, 255, 255, 255);
  }
}

void cleanup_graphics(GraphicsEngine *ge)
{
  int i;

  if (!ge)
    return;

  if (ge->spritesheet)
  {
    SDL_DestroyTexture(ge->spritesheet);
  }

  if (ge->fontsheet)
  {
    SDL_DestroyTexture(ge->fontsheet);
  }

  if (ge->renderer)
  {
    SDL_DestroyRenderer(ge->renderer);
  }

  if (ge->window)
  {
    SDL_DestroyWindow(ge->window);
  }

  if (ge->activeAnimations)
  {
    for (i = 0; i < MAX_ANIMATION_COUNT; i++)
    {
      if (ge->activeAnimations[i] != NULL)
      {
        free(ge->activeAnimations[i]);
      }
    }
    free(ge->activeAnimations);
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

void draw_ascii_char(GraphicsEngine *ge, char c, int worldX, int worldY)
{
  SDL_Rect spriteClip;
  SDL_Rect destRect;

  spriteClip.x = (c - ' ') * FONT_SIZE;
  spriteClip.y = 0;
  spriteClip.w = FONT_SIZE;
  spriteClip.h = FONT_SIZE;

  destRect.x = worldX * FONT_SIZE;
  destRect.y = worldY * FONT_SIZE;
  destRect.w = FONT_SIZE;
  destRect.h = FONT_SIZE;

  SDL_RenderCopyEx(ge->renderer, ge->fontsheet, &spriteClip, &destRect, 0, NULL, 0);
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

void present_frame(GraphicsEngine *ge)
{
  SDL_RenderPresent(ge->renderer);
}

void render_animations(GraphicsEngine *ge)
{
  int x;
  /**
   * Find active animation slots
   */
  for (x = 0; x < MAX_ANIMATION_COUNT; x++)
  {
    /* Check binary flags for active slots (active if bit = 0) */
    if (((1 << x) & ge->animationSlots) == 0)
    {
      ge->activeAnimations[x]->play(ge, ge->activeAnimations[x]);
      ge->activeAnimations[x]->currentFrame++;

      /* If the animation is over, free it and open up the slot */
      if (ge->activeAnimations[x]->currentFrame >= ge->activeAnimations[x]->duration)
      {
        free(ge->activeAnimations[x]);
        ge->activeAnimations[x] = NULL;
        ge->animationSlots = ge->animationSlots | (1 << x);
      }
    }
  }
}

void render(Game *g)
{
  clear_screen(g->graphics);
  draw_level(g->graphics, g->level);
  if (g->ui->visible)
  {
    draw_ui(g);
  }
  render_animations(g->graphics);
  SDL_RenderPresent(g->graphics->renderer);
}