#include "main.h"

UI *initialize_ui()
{
  UI *ui = (UI *)calloc(1, sizeof(UI));
  ui->message = NULL;
  ui->visible = 0;
  return ui;
}

void cleanup_ui(UI *ui)
{
  if (ui->message)
  {
    free(ui->message);
  }
  free(ui);
}

void draw_popup_background(Game *game)
{
  int endX = WORLD_WIDTH_SPRITES - 1;
  int startY = WORLD_HEIGHT_SPRITES - POPUP_HEIGHT;
  int endY = WORLD_HEIGHT_SPRITES - 1;
  int cornerXs[] = {18, 20, 18, 20};
  int cornerYs[] = {0, 0, 2, 2};
  int edgeXs[] = {19, 20, 19, 18};
  int edgeYs[] = {0, 1, 2, 1};

  Sprite sprite;
  int x, y, cornerIndex;

  sprite.angle = 0;
  sprite.flip = 0;

  for (y = startY; y <= endY; y++)
  {
    for (x = 0; x <= endX; x++)
    {
      if ((x == 0 && y == startY) || (x == endX && y == startY) ||
          (x == 0 && y == endY) || (x == endX && y == endY))
      {
        cornerIndex = (y == startY ? 0 : 2) + (x == endX ? 1 : 0);
        sprite.spriteX = cornerXs[cornerIndex];
        sprite.spriteY = cornerYs[cornerIndex];
      }
      else if (y == startY)
      {
        sprite.spriteX = edgeXs[0];
        sprite.spriteY = edgeYs[0];
      }
      else if (x == endX)
      {
        sprite.spriteX = edgeXs[1];
        sprite.spriteY = edgeYs[1];
      }
      else if (y == endY)
      {
        sprite.spriteX = edgeXs[2];
        sprite.spriteY = edgeYs[2];
      }
      else if (x == 0)
      {
        sprite.spriteX = edgeXs[3];
        sprite.spriteY = edgeYs[3];
      }
      else
      {
        sprite.spriteX = 0;
        sprite.spriteY = 0;
      }

      draw_sprite(game->graphics, &sprite, x, y);
    }
  }
}

void draw_text(Game *game, int worldX, int worldY, int lineLength, char *message)
{
  int x, y;
  int msgIndex = 0;
  int msgSpaceWidth = WORLD_WIDTH_SPRITES - 4;
  int endX = worldX + lineLength < WORLD_WIDTH_SPRITES ? worldX + lineLength : WORLD_WIDTH_SPRITES;
  int endY = WORLD_HEIGHT_SPRITES - 1;

  for (y = worldY; y < endY; y++)
  {
    for (x = 0; x < endX; x++)
    {
      msgIndex = ((y - worldY) * msgSpaceWidth) + x;
      if (message[msgIndex] == '\0')
      {
        return;
      }
      else
      {
        draw_ascii_char(game->graphics, message[msgIndex], x + worldX, y);
      }
    }
  }
}

void draw_dialog(Game *game)
{
  draw_popup_background(game);
  draw_text(game, 2, WORLD_HEIGHT_SPRITES - POPUP_HEIGHT + 1 + HUD_HEIGHT_SPRITES, WORLD_WIDTH_SPRITES - 4, game->ui->message);
}

void draw_hud(Game *game)
{
  char *s = "Health:10";
  draw_text(game, 1, 1, WORLD_WIDTH_SPRITES - 4, s);
}

void show_popup(Game *game, char *message)
{
  if (game->ui->message)
  {
    free(game->ui->message);
  }

  game->ui->message = (char *)malloc(strlen(message) * sizeof(char));
  strcpy(game->ui->message, message);
  game->ui->visible = 1;
  game->state = DIALOG_OPEN;
}

void dismiss_popup(Game *game)
{
  game->ui->visible = 0;
  if (game->ui->message)
  {
    free(game->ui->message);
    game->ui->message = NULL;
  }
}