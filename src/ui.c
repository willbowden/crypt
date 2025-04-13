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

void draw_popup(Game *game)
{
  int endX = WINDOW_WIDTH_SPRITES - 1;
  int startY = WINDOW_HEIGHT_SPRITES - POPUP_HEIGHT;
  int endY = WINDOW_HEIGHT_SPRITES - 1;
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

void draw_text(Game *game, int textX, int textY, int lineLength)
{
  int x, y;
  int msgIndex = 0;
  int msgSpaceWidth = WINDOW_WIDTH_SPRITES - 4;
  int endX = textX + lineLength < WINDOW_WIDTH_SPRITES ? textX + lineLength : WINDOW_WIDTH_SPRITES;
  int endY = WINDOW_HEIGHT_SPRITES - 1;

  for (y = textY; y < endY; y++)
  {
    for (x = textX; x < endX; x++)
    {
      msgIndex = ((y - textY) * msgSpaceWidth) + (x - 2);
      if (game->ui->message[msgIndex] == '\0')
      {
        return;
      }
      else
      {
        draw_ascii_char(game->graphics, game->ui->message[msgIndex], x, y);
      }
    }
  }
}

void draw_ui(Game *game)
{
  draw_popup(game);
  draw_text(game, 2, WINDOW_HEIGHT_SPRITES - POPUP_HEIGHT + 2, WINDOW_WIDTH_SPRITES - 4);
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