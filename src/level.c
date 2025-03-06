#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void cleanup_background(Level *level)
{
  int x, y;

  if (level->background)
  {
    for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
    {
      if (level->background[y])
      {
        for (x = 0; x < WINDOW_WIDTH_SPRITES; x++)
        {
          /* Free the background sprites */
          if (level->background[y][x])
          {
            free(level->background[y][x]);
          }
        }

        free(level->background[y]);
      }
    }

    free(level->background);
  }
}

void cleanup_foreground(Level *level)
{
  int y;

  if (level->foreground)
  {
    for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
    {
      if (level->foreground[y])
      {
        free(level->foreground[y]);
      }
    }

    free(level->foreground);
  }
}

void cleanup_level(Level *level)
{
  if (!level)
    return;

  cleanup_background(level);
  cleanup_foreground(level);

  free(level);
}

Level *create_empty_level()
{
  int y;

  Level *level = (Level *)calloc(1, sizeof(Level));
  if (!level)
  {
    fprintf(stderr, "%s\n", "Error allocating mem for empty level.");
    return NULL;
  }

  level->background = (Sprite ***)calloc(WINDOW_HEIGHT_SPRITES, sizeof(Sprite **));
  if (!level->background)
  {
    fprintf(stderr, "%s\n", "Error allocating mem for empty background layer");
    return NULL;
  }

  level->foreground = (Entity ***)calloc(WINDOW_HEIGHT_SPRITES, sizeof(Entity **));
  if (!level->foreground)
  {
    fprintf(stderr, "%s\n", "Error allocating mem for empty foreground layer");
    return NULL;
  }

  for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
  {
    level->background[y] = (Sprite **)calloc(WINDOW_WIDTH_SPRITES, sizeof(Sprite *));
    if (!level->background[y])
    {
      fprintf(stderr, "Error allocating mem for empty background row %d\n", y);
      return NULL;
    }

    level->foreground[y] = (Entity **)calloc(WINDOW_WIDTH_SPRITES, sizeof(Entity *));
    if (!level->foreground[y])
    {
      fprintf(stderr, "Error allocating mem for empty foreground row %d\n", y);
      return NULL;
    }
  }

  return level;
}

/**
 * TODO: Calculate rotation/flipping from bit flags
 */
Sprite *sprite_from_number(int tileNo)
{
  int spriteX, spriteY, hflip, vflip, dflip;
  Sprite *newSprite;

  if (tileNo == -1)
  {
    return NULL;
  }

  hflip = tileNo & 1 << 31;
  vflip = tileNo & 1 << 30;
  dflip = tileNo & 1 << 29;

  tileNo &= ~(1 << 31 | 1 << 30 | 1 << 29);

  spriteX = ((tileNo + 1) % SPRITESHEET_WIDTH_SPRITES) - 1;
  spriteY = (int)tileNo / SPRITESHEET_WIDTH_SPRITES;

  newSprite = (Sprite *)calloc(1, sizeof(Sprite));
  newSprite->spriteX = spriteX;
  newSprite->spriteY = spriteY;
  newSprite->angle = 0;
  newSprite->flip = SDL_FLIP_NONE;

  /**
   * TODO: Still figuring out flips
   */

  if (dflip)
  {
    if (hflip)
    {
      newSprite->angle = 90.0;
    }
    else if (vflip)
    {
      newSprite->angle = 270.0;
    }
  }
  else if (hflip && vflip)
  {
    newSprite->angle = 180.0;
  }
  else if (hflip)
  {
    newSprite->flip = SDL_FLIP_HORIZONTAL;
  }
  else if (vflip)
  {
    newSprite->flip = SDL_FLIP_VERTICAL;
  }

  return newSprite;
}

Entity *entity_from_number(int tileNo)
{
  Sprite *sprite;

  if (tileNo == -1)
  {
    return NULL;
  }

  /**
   * TODO: Add inequalities to match different tile numbers
   * to different entity types.
   *
   * For now, all tiles are assumed to be ForegroundTiles
   */

  sprite = sprite_from_number(tileNo);

  return (Entity *)create_foreground_tile(sprite, 0);
}

int load_layer(LEVEL_LAYER layer, char *levelPrefix, char *levelSuffix, ENTITY_FACTORY func)
{
  char *levelName = (char *)malloc((strlen(levelPrefix) + 15) * sizeof(char));
  FILE *file = NULL;
  int tileNo = -1;
  int x, y, successfulReads = 0;

  strcpy(levelName, levelPrefix);
  strcat(levelName, levelSuffix);

  file = fopen(levelName, "r");

  if (!file)
  {
    fprintf(stderr, "Error opening level file: %s\n", levelName);
    return -1;
  }

  free(levelName);

  for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
  {
    for (x = 0; x < WINDOW_WIDTH_SPRITES; x++)
    {
      if (fscanf(file, "%d,", &tileNo) == EOF)
      {
        if (successfulReads < WINDOW_HEIGHT_SPRITES * WINDOW_WIDTH_SPRITES)
        {
          fprintf(stderr, "Error reading background tile at location x:%d y%d:\n", x, y);
          fclose(file);
          return -1;
        }
      }

      layer[y][x] = (void *)func(tileNo);
    }
  }

  fclose(file);
  return 0;
}

Level *load_level(char *levelName)
{
  char *levelPrefix = (char *)malloc((strlen(levelName) + 15) * sizeof(char));
  Level *level = create_empty_level();

  strcat(levelPrefix, levelName);

  if (load_layer(
          (LEVEL_LAYER)level->background,
          levelName,
          "_Background.csv",
          (ENTITY_FACTORY)sprite_from_number) < 0)
  {
    free(levelPrefix);
    return NULL;
  }

  if (load_layer(
          (LEVEL_LAYER)level->foreground,
          levelName,
          "_Foreground.csv",
          (ENTITY_FACTORY)entity_from_number) < 0)
  {
    free(levelPrefix);
    return NULL;
  }

  free(levelPrefix);
  return level;
}
