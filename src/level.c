#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void cleanup_level(Level *level)
{
  int y;

  if (!level)
    return;

  if (level->background)
  {
    for (y = 0; y < WINDOW_HEIGHT_SPRITES; y++)
    {
      if (level->background[y])
      {
        free(level->background[y]);
      }
    }

    free(level->background);
  }

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

  level->foreground = (GenericTile ***)calloc(WINDOW_HEIGHT_SPRITES, sizeof(GenericTile **));
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

    level->foreground[y] = (GenericTile **)calloc(WINDOW_WIDTH_SPRITES, sizeof(GenericTile *));
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
  int spriteX, spriteY;
  Sprite *newSprite;

  if (tileNo == -1)
  {
    return NULL;
  }

  spriteX = ((tileNo + 1) % SPRITESHEET_WIDTH_SPRITES) - 1;
  spriteY = (int)tileNo / SPRITESHEET_WIDTH_SPRITES;

  newSprite = (Sprite *)calloc(1, sizeof(Sprite));
  newSprite->spriteX = spriteX;
  newSprite->spriteY = spriteY;
  newSprite->angle = 0.0;
  newSprite->flip = SDL_FLIP_NONE;

  return newSprite;
}

int load_background(Level *level, char *levelName)
{
  char suffix[] = "_Background.csv";
  FILE *file = NULL;
  int tileNo = -1;
  int x, y, successfulReads = 0;

  strcat(levelName, suffix);

  file = fopen(levelName, "r");

  if (!file)
  {
    fprintf(stderr, "Error opening level file: %s\n", levelName);
    return -1;
  }

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
      level->background[y][x] = sprite_from_number(tileNo);
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

  if (load_background(level, levelPrefix) < 0)
  {
    free(levelPrefix);
    return NULL;
  }

  free(levelPrefix);
  return level;
}
