#include "main.h"

Menu *initialise_menu()
{
  Menu *menu = (Menu *)malloc(sizeof(Menu));
  MenuItem new_game_item = {
      &new_game,
      "New Game"};

  MenuItem load_game_item = {
      &load_game,
      "Load Save"};

  menu->title = "Crypt";
  menu->text = "New game or resume from save?";
  menu->num_items = 2;
  menu->selected_item = 0;
  menu->menu_items[0] = new_game_item;
  menu->menu_items[1] = load_game_item;
  menu->selector = sprite_from_number(573);

  return menu;
}

void setup_game_over_menu(Menu *menu)
{
  MenuItem restart_game_item = {
      &new_game,
      "Restart game"};

  MenuItem quit_game_item = {
      (int (*)(Game *))&quit_game,
      "Admit defeat."};

  menu->title = "Game Over";
  menu->text = "You died. Bad luck.";
  menu->num_items = 2;
  menu->selected_item = 0;
  menu->menu_items[0] = restart_game_item;
  menu->menu_items[1] = quit_game_item;
  free(menu->selector);
  menu->selector = sprite_from_number(622);
}

void setup_game_win_menu(Menu *menu)
{
  MenuItem play_again_item = {
      &new_game,
      "Play Again!"};

  MenuItem quit_game_item = {
      (int (*)(Game *))&quit_game,
      "Quit game"};

  menu->title = "You win!";
  menu->text = "Congratulations!";
  menu->num_items = 2;
  menu->selected_item = 0;
  menu->menu_items[0] = play_again_item;
  menu->menu_items[1] = quit_game_item;
  free(menu->selector);
  menu->selector = sprite_from_number(142);
}

void cleanup_menu(Menu *menu)
{
  free(menu->selector);
}

/**
 * Draw a brick-bordered blank box to fill with UI elements
 */
void draw_popup_background(Game *game, int startX, int startY, int endX, int endY)
{
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
    for (x = startX; x <= endX; x++)
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

/**
 * Draw ASCII text anywhere on screen
 */
void draw_text(Game *game, int screenX, int screenY, int lineLength, char *message)
{
  int x, y;
  int msgIndex = 0;
  int endX = screenX + lineLength <= WORLD_WIDTH_SPRITES ? screenX + lineLength : WORLD_WIDTH_SPRITES;
  int endY = WORLD_HEIGHT_SPRITES - 1;

  for (y = screenY; y < endY; y++)
  {
    for (x = screenX; x < endX; x++)
    {
      msgIndex = ((y - screenY) * lineLength) + (x - screenX);
      if (message[msgIndex] == '\0')
      {
        return;
      }
      else
      {
        draw_ascii_char(game->graphics, message[msgIndex], x, y);
      }
    }
  }
}

/**
 * Draw a box occupying the bottom half of the screen with a message on top
 */
void draw_dialog(Game *game)
{
  draw_popup_background(game, 0, SCREEN_HEIGHT_SPRITES - POPUP_HEIGHT, SCREEN_WIDTH_SPRITES - 1, SCREEN_HEIGHT_SPRITES - 1);
  draw_text(game, 2, SCREEN_HEIGHT_SPRITES - POPUP_HEIGHT + 2, SCREEN_WIDTH_SPRITES - 4, game->popup.message);
}

/**
 * Draw the HUD above the world to display player info
 */
void draw_hud(Game *game)
{
  char hp[7];
  char atk[8];
  char def[8];

  sprintf(hp, "HP:%d", game->player->health);
  sprintf(atk, "ATK:%d", game->player->attack);
  sprintf(def, "DEF:%d", game->player->defense);

  draw_text(game, 1, 1, SCREEN_WIDTH_SPRITES - 4, hp);
  draw_text(game, 8, 1, SCREEN_WIDTH_SPRITES - 4, atk);
  draw_text(game, 16, 1, SCREEN_WIDTH_SPRITES - 4, def);
}

/**
 * Draw a fullscreen menu with a list of actions to take
 */
void draw_menu(Game *game)
{
  int i, row_length, is_selected, x, y;
  int longest_length = 0;
  MenuItem *menu_item;
  int title_length = strlen(game->menu->title);
  int line_length = SCREEN_WIDTH_SPRITES - 2;
  int text_length = strlen(game->menu->text);
  int items_y = (((strlen(game->menu->text) / line_length) + 1) * 2) + 4;
  
  draw_popup_background(game, 0, 0, SCREEN_WIDTH_SPRITES - 1, SCREEN_HEIGHT_SPRITES - 1);
  draw_text(game, (SCREEN_WIDTH_SPRITES / 2 - title_length / 2), 2, title_length, game->menu->title);
  draw_text(game, (SCREEN_WIDTH_SPRITES / 2) - (text_length / 2), 4, line_length, game->menu->text);

  for (i = 0; i < game->menu->num_items; i++)
  {
    menu_item = &game->menu->menu_items[i];
    row_length = strlen(menu_item->text);
    if (row_length > longest_length)
    {
      longest_length = row_length;
    }
  }

  for (i = 0; i < game->menu->num_items; i++)
  {
    menu_item = &game->menu->menu_items[i];
    is_selected = i == game->menu->selected_item;
    y = items_y + (2 * i);
    x = (SCREEN_WIDTH_SPRITES / 2) - (longest_length / 2) - 1;

    /**
     * Draw the 'cursor' sprite next to the currently highlighted element
     */
    if (is_selected)
    {
      draw_sprite(game->graphics, game->menu->selector, x - 2, y);
    }

    draw_text(game, x, y, longest_length, menu_item->text);
  }
}

void show_popup(Game *game, char *message)
{
  if (game->popup.message)
  {
    free(game->popup.message);
  }

  game->popup.message = (char *)malloc(strlen(message) * sizeof(char));
  strcpy(game->popup.message, message);
  game->popup.visible = 1;
  game->state = DIALOG_OPEN;
}

void dismiss_popup(Game *game)
{
  game->popup.visible = 0;
  if (game->popup.message)
  {
    free(game->popup.message);
    game->popup.message = NULL;
  }
}